#!/usr/bin/env python3
"""
Convert rAthena's doc/*.txt reference files to Markdown.

The .txt files follow a few house conventions, which this script maps to Markdown:

    //===== rAthena Documentation =====   header block            -> title + intro
    ==== / |N. Section| / ====           section banner          -> ## N. Section
    Title / ----------                    underlined title        -> ### Title
    ---------------------------------------  (39 dashes)          -> entry separator, dropped
    *command <args>   (@command in atcommands.txt)                -> ### command + fenced signature block
    ** Title                                                      -> ### Title
    <tab>example                          tab-indented block      -> fenced code block
    <param> in prose                                              -> `<param>`

Usage:
    tools/doc2md.py doc/mapflags.txt          convert one file (writes doc/mapflags.md)
    tools/doc2md.py --all                     convert every doc/*.txt that is a reference document
    tools/doc2md.py --all --delete            ... and git rm the .txt sources

The output is a starting point: it is deterministic so tools/merge-upstream.sh can apply it to
upstream's copies, but the result is meant to be tidied by hand afterwards.
"""
import argparse
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
DOC = ROOT / "doc"

# doc/*.txt files that are not reference documents (scripts, data, third party) stay as they are
SKIP = set()

SEPARATOR_WIDTH = 39
PARAM_RE = re.compile(r"<([^<>`\n]+?)>")
DOC_REF_RE = re.compile(r"doc/([A-Za-z0-9_]+)\.txt")
ENTRY_NAME_RE = re.compile(r"^[*@#]([A-Za-z_][A-Za-z0-9_]*)")


def is_code(line):
    """Tab-indented, tab-separated or deeply space-indented lines are examples/tables."""
    return line.startswith("\t") or "\t" in line or line.startswith("    ")


def looks_like_prose(text):
    """A sentence rather than code or a key/value line."""
    t = text.strip()
    return len(t.split()) >= 5 and ";" not in t and "//" not in t and not re.match(r"^\S+\s*=", t)


def is_dashes(line):
    return len(line) >= 3 and set(line) == {"-"}


def is_equals(line):
    return len(line) >= 3 and set(line) == {"="}


def prose(line, converted_names):
    """Escape a line of running text for Markdown."""
    parts = line.split("`")
    for k in range(0, len(parts), 2):  # even parts are outside code spans
        parts[k] = PARAM_RE.sub(r"`<\1>`", parts[k])
    line = "`".join(parts)
    if line.startswith("#"):
        line = "\\" + line
    return line.rstrip()


def parse_header(lines):
    """Parse the leading //===== block. Returns (fields, index of first body line)."""
    fields = {}
    key = None
    i = 0
    while i < len(lines) and lines[i].startswith("//"):
        line = lines[i]
        m = re.match(r"//=+\s*(.*?)\s*:?\s*=+\s*$", line)
        if m and line.startswith("//====="):
            key = m.group(1).strip() or None
            if key is not None:
                fields.setdefault(key, [])
        elif key is not None:
            m = re.match(r"//=\s?(.*)$", line)
            if m:
                fields[key].append(m.group(1).rstrip())
        i += 1
    return fields, i


ENTRY_STYLES = {
    # file name -> (regex matching an entry line, keep prefix in heading, signature block)
    "atcommands.txt": (re.compile(r"^@[A-Za-z_]"), True, True),
    "status_change.txt": (re.compile(r"^SC_[A-Z0-9_]+\t"), True, False),
    "packet_interserv.txt": (re.compile(r"^0x[0-9A-Fa-f]+:"), True, False),
}
DEFAULT_STYLE = (re.compile(r"^\*(?![*\s])"), False, True)
SUBSECTION_RE = re.compile(r"^\*{1,2}\s+(\S.*)$")
# 'nothing  - A permanent variable ...', '"#"  - ...', 'BaseClass - ...': hand-made definition lists
DEFINITION_RE = re.compile(r"^(\S[^\s\t]{0,19})\s+-\s+(\S.*)$")
TABLE_RULE_RE = re.compile(r"^\s*-{3,}(\s+-{3,})+\s*$")
LIST_ITEM_RE = re.compile(r"^\s*(- |\d+[.)] )")
OPERATOR_LINE_RE = re.compile(r"^([-+*])\s+-\s")


def convert(text, entry_style, converted_names):
    entry_re, keep_prefix, with_block = entry_style
    lines = text.split("\n")
    fields, i = parse_header(lines)
    out = []

    title = " ".join(fields.get("rAthena Documentation", [])).strip()
    out.append(f"# {title}" if title else "# " + "Untitled")
    out.append("")
    description = [l for l in fields.get("Description", []) if l.strip()]
    if description:
        out.extend(prose(l, converted_names) for l in description)
        out.append("")
    meta = []
    if fields.get("Last Updated"):
        meta.append(f"Last updated {fields['Last Updated'][0].strip()}")
    if fields.get("By"):
        meta.append("by " + ", ".join(l.strip() for l in fields["By"] if l.strip()))
    if meta:
        out.append("*" + " ".join(meta) + " (upstream `doc/` header)*")
        out.append("")
    for key, value in fields.items():
        if key in ("rAthena Documentation", "Description", "Last Updated", "By"):
            continue
        if not any(l.strip() for l in value):
            continue
        out.append(f"**{key}**")
        out.append("")
        out.append("```")
        out.extend(l.replace("\t", "    ").rstrip() for l in value)
        out.append("```")
        out.append("")

    def blank():
        if out and out[-1] != "":
            out.append("")

    n = len(lines)
    while i < n:
        line = lines[i].rstrip("\r")

        # section banner: ===== / |Title| / =====   (indented banners are sub-sections)
        if is_equals(line.strip()) and i + 2 < n and lines[i + 1].strip().startswith("|") and lines[i + 1].rstrip().endswith("|") and is_equals(lines[i + 2].strip()):
            heading = lines[i + 1].strip().strip("|").strip()
            m = re.match(r"^(\d+(?:\.\d+)*)\.-?\s*(.*?)\.?$", heading)
            if m:
                heading = f"{m.group(1)}. {m.group(2)}"
            blank()
            out.append(("### " if line.startswith((" ", "\t")) else "## ") + heading)
            out.append("")
            i += 3
            continue

        # aligned table: a header line followed by an underline row with one dash group per column
        if i + 2 < n and TABLE_RULE_RE.match(lines[i + 1]) and lines[i + 2].strip() and line.strip() and not is_dashes(line.strip()):
            rule = lines[i + 1].rstrip("\r")
            starts = [m.start() for m in re.finditer(r"-{3,}", rule)]
            def cells(text):
                text = text.rstrip("\r").expandtabs(8)
                return [text[a:b].strip() for a, b in zip(starts, starts[1:] + [None])]
            header = cells(line)
            rows = []
            j = i + 2
            while j < n and lines[j].strip() and not TABLE_RULE_RE.match(lines[j]) and not is_equals(lines[j].strip()):
                row = cells(lines[j])
                if rows and row and not row[0] and any(row):
                    # continuation line: append to the last filled cell of the previous row
                    k = max(idx for idx, c in enumerate(row) if c)
                    rows[-1][k] = (rows[-1][k] + " " + row[k]).strip()
                else:
                    rows.append(row)
                j += 1
            blank()
            esc = lambda c: prose(c, converted_names).replace("|", "\\|")
            out.append("| " + " | ".join(esc(c) for c in header) + " |")
            out.append("|" + "|".join("---" for _ in header) + "|")
            for row in rows:
                out.append("| " + " | ".join(esc(c) for c in row) + " |")
            out.append("")
            i = j
            continue

        # continuation of a list item or definition item: indented lines right after it (no blank line)
        if line.startswith(("    ", "\t")) and out and (LIST_ITEM_RE.match(out[-1]) or out[-1].startswith("  ")) and lines[i - 1].strip() != "" and looks_like_prose(line):
            out.append("  " + prose(line.strip(), converted_names))
            i += 1
            continue

        # 'Useful functions:' / 'aggregation:' followed by 2-3 space indented code lines
        if re.match(r"^ {2,3}\S", line) and not looks_like_prose(line) and not LIST_ITEM_RE.match(line) and (lines[i - 1].rstrip().endswith(":") or (out and out[-1] == "```" and False)):
            block = []
            j = i
            while j < n and re.match(r"^ {2,}\S", lines[j]) and not looks_like_prose(lines[j]):
                block.append(lines[j].rstrip("\r").strip())
                j += 1
            blank()
            out.append("```")
            out.extend(block)
            out.append("```")
            out.append("")
            i = j
            continue

        # continuation of a definition item: indented lines right after it (no blank line)
        if line.startswith(("    ", "\t")) and out and (out[-1].startswith("- `") or out[-1].startswith("  ")) and lines[i - 1].strip() != "":
            out.append("  " + prose(line.strip(), converted_names))
            i += 1
            continue

        # tab-indented block -> code
        if is_code(line) and not entry_re.match(line):
            block = []
            j = i
            while j < n:
                l = lines[j].rstrip("\r")
                if is_code(l) and not entry_re.match(l):
                    block.append(l.rstrip())
                    j += 1
                elif l.strip() == "":
                    # keep a blank line only if the block continues afterwards
                    k = j
                    while k < n and lines[k].strip() == "":
                        k += 1
                    if k < n and is_code(lines[k]) and not entry_re.match(lines[k]):
                        block.extend([""] * (k - j))
                        j = k
                    else:
                        break
                else:
                    break
            # strip one level of tab indentation when every line has it
            if block and all(l.startswith("\t") or l == "" for l in block):
                block = [l[1:] if l.startswith("\t") else l for l in block]
            blank()
            out.append("```")
            out.extend(block)
            out.append("```")
            out.append("")
            i = j
            continue

        # entries: one or more consecutive *signature lines
        if entry_re.match(line) and re.match(r"^\*[A-Za-z]+:\s*$", line):
            blank()
            out.append("### " + line[1:].strip().rstrip(":"))
            out.append("")
            i += 1
            continue

        if entry_re.match(line):
            sigs = []
            j = i
            while j < n and entry_re.match(lines[j]):
                sigs.append(lines[j].rstrip("\r").rstrip())
                j += 1
            m = re.match(r"^([*@]?[A-Za-z_][A-Za-z0-9_]*|0x[0-9A-Fa-f]+)", sigs[0])
            name = m.group(1) if m else sigs[0].strip()
            if not keep_prefix:
                name = name.lstrip("*")
            if entry_style is ENTRY_STYLES.get("status_change.txt"):
                efst = re.search(r"\((.*?)\)", sigs[0])
                if efst and efst.group(1).strip():
                    name = f"{name} ({efst.group(1).strip()})"
            signatures = [s[1:] if not keep_prefix else s for s in sigs]
            names = [name]
            for sig in signatures:
                m2 = re.match(r"^(@?[A-Za-z_][A-Za-z0-9_]*)", sig)
                if m2 and m2.group(1) not in names and with_block:
                    names.append(m2.group(1))
            blank()
            for extra in names[1:]:
                out.append(f'<a name="{extra.lstrip("@")}"></a>')
            out.append("### " + " / ".join(names))
            out.append("")
            # a bare name carries no information beyond the heading
            if with_block and signatures != [name]:
                out.append("```")
                out.extend(signatures)
                out.append("```")
                out.append("")
            i = j
            continue

        # '+ - adds', '- - subtracts', '* - multiplies': prose, not a list
        if OPERATOR_LINE_RE.match(line):
            out.append("\\" + prose(line, converted_names))
            i += 1
            continue

        # ** Subsection / * Subsection
        m = SUBSECTION_RE.match(line)
        if m:
            blank()
            out.append("### " + m.group(1).strip().rstrip(":.").strip())
            out.append("")
            i += 1
            continue

        # dashes: separator or underline
        if is_dashes(line):
            prev = lines[i - 1].rstrip("\r") if i > 0 else ""
            if len(line) != SEPARATOR_WIDTH and prev.strip() and not prev.startswith("\t") and not is_dashes(prev) and not is_equals(prev) and out and out[-1] == prose(prev, converted_names):
                out[-1] = "### " + prev.strip().rstrip(":")
                # a heading needs a blank line before it
                if len(out) >= 2 and out[-2] != "":
                    out.insert(len(out) - 1, "")
                out.append("")
            else:
                blank()
            i += 1
            continue

        if line.strip() == "":
            blank()
            i += 1
            continue

        # definition list item
        m = DEFINITION_RE.match(line)
        if m and not line.startswith(("- ", "\\")):
            key = m.group(1)
            out.append(f"- `{key}` - {prose(m.group(2), converted_names)}")
            i += 1
            continue

        # the "search for *name" hint only makes sense in a text file
        if re.match(r"^To (search for|find) .*(write|put) .*before", line):
            i += 1
            continue

        out.append(prose(line, converted_names))
        i += 1

    # trim
    while out and out[-1] == "":
        out.pop()
    text = "\n".join(out) + "\n"
    # cross references, code blocks included: 'doc/x.txt' -> 'doc/x.md' for every doc that is (or becomes) Markdown
    text = DOC_REF_RE.sub(lambda m: f"doc/{m.group(1)}.md" if m.group(1) in converted_names else m.group(0), text)
    return text


def reference_docs():
    return sorted(p for p in DOC.glob("*.txt") if p.name not in SKIP)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", type=pathlib.Path)
    ap.add_argument("--all", action="store_true", help="convert every reference document in doc/")
    ap.add_argument("--delete", action="store_true", help="git rm the .txt source after converting")
    args = ap.parse_args()

    files = reference_docs() if args.all else args.files
    if not files:
        ap.error("nothing to convert")
    converted_names = {p.stem for p in reference_docs()} | {p.stem for p in DOC.glob("*.md")}
    for src in files:
        src = src.resolve()
        entry_style = ENTRY_STYLES.get(src.name, DEFAULT_STYLE)
        text = src.read_text(encoding="utf-8", errors="replace")
        dst = src.with_suffix(".md")
        dst.write_text(convert(text, entry_style, converted_names), encoding="utf-8")
        if args.delete:
            subprocess.run(["git", "rm", "-q", "--cached", str(src)], check=False, cwd=ROOT)
            src.unlink()
        print(f"{src.relative_to(ROOT)} -> {dst.relative_to(ROOT)}")


if __name__ == "__main__":
    sys.exit(main())

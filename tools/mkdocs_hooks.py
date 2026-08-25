"""
MkDocs hooks (configured in mkdocs.yml).

doc/script_commands.md is kept as one file in the repository so that upstream's edits
to doc/script_commands.txt can be merged onto it. For the site it is far too long for a
single page, so this hook splits it on its '## N. Section' headings into virtual pages
under script_commands/ and expands the 'script_commands/' placeholder in the nav.

'sample/' in the nav is expanded to the files in doc/sample/, shown as code.
"""
import re
from pathlib import Path

from mkdocs.structure.files import File

SPLIT_SOURCE = "script_commands.md"
SPLIT_DIR = "script_commands"
SAMPLE_DIR = "sample"

_generated = {}  # src_uri -> content


def _slug(title):
    return re.sub(r"[^a-z0-9]+", "-", title.lower()).strip("-")


def _split_sections(text):
    """Yield (title, body) for the intro and each '## ' section."""
    parts = re.split(r"^(## .+)$", text, flags=re.M)
    yield "Introduction", parts[0]
    for i in range(1, len(parts), 2):
        yield parts[i][3:].strip(), parts[i] + "\n" + parts[i + 1]


def on_config(config):
    docs = Path(config["docs_dir"])
    _generated.clear()

    # --- script commands: one page per section
    text = (docs / SPLIT_SOURCE).read_text(encoding="utf-8")
    pages = []
    index = ["# Script commands", "", "The reference manual for the rAthena scripting language, one page per section.", "", ]
    for n, (title, body) in enumerate(_split_sections(text)):
        name = f"{_slug(title)}.md"
        uri = f"{SPLIT_DIR}/{name}"
        heading = re.sub(r"^\d+(\.\d+)*\.\s*", "", title)
        if n == 0:
            page = body
        else:
            page = re.sub(r"^## .+$", f"# {title}", body, count=1, flags=re.M)
            # entries become second level headings on their own page
            page = re.sub(r"^### ", "## ", page, flags=re.M)
        _generated[uri] = page
        pages.append({heading if n else "Introduction": uri})
        index.append(f"- [{title}]({name})")
    index.append("")
    index.append(f"The whole reference on a single page: [script_commands.md](../{SPLIT_SOURCE}).")
    _generated[f"{SPLIT_DIR}/index.md"] = "\n".join(index) + "\n"
    script_nav = [{"Overview": f"{SPLIT_DIR}/index.md"}] + pages + [{"Single page": SPLIT_SOURCE}]

    # --- samples: show the .txt scripts as code pages
    sample_nav = []
    for f in sorted((docs / SAMPLE_DIR).glob("*.txt")):
        uri = f"{SAMPLE_DIR}/{f.stem}.md"
        _generated[uri] = f"# {f.stem}\n\n```\n{f.read_text(encoding='utf-8', errors='replace')}\n```\n"
        sample_nav.append({f.stem: uri})

    def expand(nav):
        for item in nav:
            for key, value in item.items():
                if value == f"{SPLIT_DIR}/":
                    item[key] = script_nav
                elif value == f"{SAMPLE_DIR}/":
                    item[key] = sample_nav
                elif isinstance(value, list):
                    expand(value)

    expand(config["nav"])
    return config


def on_files(files, config):
    for uri, content in _generated.items():
        files.append(File.generated(config, uri, content=content))
    return files

# Code style

All C++ under `src/` is formatted with [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
using the rules in [`.clang-format`](../.clang-format). CI rejects unformatted files
(`.github/workflows/code_style.yml`), so run the formatter before committing:

```sh
tools/format.sh              # format everything
tools/format.sh src/map      # or just a directory / file
tools/format.sh --check      # what CI runs
```

Most editors apply `.clang-format` automatically (VS Code: C/C++ extension, "format on save";
Visual Studio: Tools > Options > Text Editor > C/C++ > Formatting; CLion: enables it when the
file is present). Use clang-format 22 — other versions produce slightly different output.

## The rules

The configuration is derived from two published style guides and adjusted where the rAthena
tree already had a clear majority convention:

- [WebKit Code Style Guidelines](https://webkit.org/code-style-guidelines/) — spacing, pointer
  placement, short statements, no hard column limit.
- [Linux kernel coding style](https://www.kernel.org/doc/html/latest/process/coding-style.html) —
  tabs for indentation, K&R braces, space after control keywords.

| Rule | Value | Source |
|---|---|---|
| Indentation | one tab per level, displayed as 4 columns; spaces only for alignment | kernel, `.editorconfig` |
| Braces | opening brace on the same line, also for functions and classes; `} else {` | rAthena majority (kernel/WebKit put function braces on their own line) |
| Control statements | `if (x) {` — space after the keyword, none inside the parentheses | WebKit, kernel |
| Function calls | `f(a, b)` — no space inside the parentheses | WebKit, kernel |
| Pointers / references | `T* p`, `T& r` | WebKit |
| `switch` | `case` labels indented one level, bodies one more | rAthena majority |
| Short statements | `case X: return y;` may stay on one line; `if`/loops never | WebKit (relaxed for `case`) |
| Column limit | none — existing line breaks are kept, long lines are not rewrapped | WebKit |
| Blank lines | at most one in a row; none at the start of a block or file | WebKit, kernel |
| Includes | never reordered (`SortIncludes: Never`) | rAthena: include order is significant in places |
| Namespaces | contents indented; closing brace gets `// namespace x` | rAthena majority |
| Line endings | LF, newline at end of file | `.editorconfig` |

Everything else (operator spacing, casts, templates, lambdas, constructor initialisers) is the
WebKit default.

Naming is not enforced by tooling. Follow the surrounding code: `snake_case` for functions and
variables, the established short names (`sd`, `tsd`, `md`, `nd`, `bl`, ...) for the usual
objects, `e_`/`s_` prefixes for enums/structs, `UPPER_CASE` for macros and enum values.

## Merging upstream

Upstream rathena/rathena is not clang-formatted, so a plain `git merge upstream/master` would
conflict on every line upstream touched since the reformat. `tools/merge-upstream.sh` avoids
that: it formats upstream's tree with this repository's `.clang-format` (and converts upstream's
`doc/*.txt` to Markdown with `tools/doc2md.py`) on a temporary branch, commits that, and merges the
*formatted* upstream. Both sides of the merge then differ only by the real changes.

```sh
tools/merge-upstream.sh            # fetches upstream/master, formats it, merges it
```

Resolve any remaining conflicts as usual, run `tools/format.sh` once more, commit.

## History

The whole-tree reformat is one commit. Its hash is listed in `.git-blame-ignore-revs`; run

```sh
git config blame.ignoreRevsFile .git-blame-ignore-revs
```

once so `git blame` skips it (GitHub's blame view honours the file automatically).

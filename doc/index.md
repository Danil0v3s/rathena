# rAthena documentation

Reference documentation for the rAthena server, built from the Markdown files in [`doc/`](https://github.com/Danil0v3s/rathena/tree/master/doc).

- **Scripting** — the [script command reference](script_commands/index.md), [item bonuses](item_bonus.md), [status changes](status_change.md) and [client effects](effect_list.md).
- **Server** — [atcommands](atcommands.md), [permissions](permissions.md), [mapflags](mapflags.md) and other operator topics.
- **Databases** — the format of the YAML databases under `db/`.
- **Development** — [source overview](source_doc.md), [code style](code_style.md) and packet documentation.

Building and installing the server is covered in the repository [README](https://github.com/Danil0v3s/rathena#readme).

## Editing

Every page has an *edit* link to its Markdown source. To preview locally:

```sh
pip install 'mkdocs-material>=9,<10'
mkdocs serve          # http://127.0.0.1:8000, reloads on save
```

`mkdocs build --strict` is what CI runs; broken links fail the build.

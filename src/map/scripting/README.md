# TS scripting host (POC)

V8-backed scripting layer for the map server. Mirrors the design of
[`mmo-csharp`'s `ScriptHost`](../../../../mmo-csharp/Map.Server/Scripting/ScriptHost.cs):
a single V8 Isolate + Context for the lifetime of the map server,
populated at boot by evaluating one bundled JS entry point. Scripts
register NPC handlers via `registerNpc({ ..., onClick })`; the host
dispatches click + dialog-resume packets into the bound handlers.

## Scope of this POC

Only one registrar and four dialog ops are wired:

| host-side | TS-side                  | resumes when                |
|-----------|--------------------------|-----------------------------|
| `clif_scriptmes`   | `await ctx.mes(text)`    | immediately (non-suspending) |
| `clif_scriptnext`  | `await ctx.next()`       | client clicks "Next"         |
| `clif_scriptmenu`  | `await ctx.select(opts)` | client picks a menu item     |
| `clif_scriptclose` | `await ctx.close()`      | client clicks "Close"        |

Everything else from the C# port (`registerShop` / `registerWarp` /
`registerSpawn` / `registerMapFlag` / `registerItem` / `registerCombo`,
plus `ctx.player` / `ctx.world` / `ctx.party` / ...) is intentionally
out of scope. Add by mirroring the existing files; the binding pattern
generalizes cleanly.

## Build

The host is **off by default**; see the *TypeScript scripting host* section of the
repository [README](../../../README.md#typescript-scripting-host-optional) for the
requirements (system V8, the v8pp submodule, Node.js, Python 3 + libclang). In short:

    git submodule update --init 3rdparty/v8pp/src
    cmake --preset dev -DENABLE_TS_SCRIPTING=ON     # -DV8_ROOT=/path if V8 is not where FindV8.cmake looks
    cmake --build --preset dev --target map-server
    cd npc-ts && npm install && npm run build       # -> npc-ts/dist/main.js

`3rdparty/cmake/FindV8.cmake` looks at Homebrew prefixes, `/usr/include/v8`, `/usr/local` and
`$V8_ROOT`. Start the map-server; on boot you should see:

    [Status]: [ts-scripting] V8 engine ready.
    [Status]: [ts-scripting] loaded bundle 'npc-ts/dist/main.js' (N NPCs registered).

The legacy script engine still runs alongside — TS gets first refusal on click. Any NPC
without a TS registration falls through to the existing `run_script()` path.

## Architecture

```
                  ┌─────────────────────┐
   ┌──────────────│   ScriptHost (C++)  │──────────────┐
   │              └─────────────────────┘              │
   │ owns                                              │ owns
   │                                                   │
   ▼                                                   ▼
┌──────────────────┐                  ┌───────────────────────────┐
│ v8::Isolate +    │                  │  DialogSessionStore       │
│ v8::Context      │                  │  (one DialogSession per   │
│ (single, global) │                  │   active dialog, keyed    │
│                  │                  │   by sd->bl.id)           │
└──────────────────┘                  └───────────────────────────┘
   │
   │ runs at boot
   ▼
┌──────────────────────────────────────────────────────────────┐
│ globalThis.registerNpc(...)  ← installed by                  │
│                                registrar_bindings.cpp        │
│                                                              │
│ TS bundle calls registerNpc({                                │
│     name: "Kafra#prt01",                                     │
│     onClick: async (ctx) => { ... }                          │
│ })                                                           │
│                                                              │
│ → entry stored in NpcRegistry, keyed by name                 │
└──────────────────────────────────────────────────────────────┘

At click time:
  npc.cpp:npc_click  →  script_host_dispatch_npc_click
                          → look up NpcRegistration by nd->name
                          → build DialogContext + JS wrapper
                          → invoke onClick(ctx)
                          → script awaits ctx.select(...)
                            → C++ creates Promise::Resolver
                            → sends ZC_MENU_LIST packet
                            → returns Promise to JS
                          → script suspends; control returns to C++

When the menu response packet arrives:
  clif.cpp:clif_parse_NpcSelectMenu
    → npc.cpp:npc_scriptcont
        → script_host_dispatch_npc_resume
            → find DialogSession by sd->bl.id
            → Resolver::Resolve(chosen index)
            → PerformMicrotaskCheckpoint → JS resumes
            → script may suspend again, or call ctx.close()
```

## File map

| file                       | role                                                    |
|----------------------------|---------------------------------------------------------|
| `script_host.{hpp,cpp}`    | engine lifecycle + public dispatch facade               |
| `npc_registry.{hpp,cpp}`   | in-memory `name → NpcRegistration` map                  |
| `registrar_bindings.{hpp,cpp}` | installs `globalThis.registerNpc`                   |
| `js_object_reader.{hpp,cpp}` | typed field accessors (parallel to JsObjectReader.cs) |
| `dialog_session.{hpp,cpp}` | per-player pending-promise state                        |
| `dialog_context.{hpp,cpp}` | the JS-side `ctx` wrapper (mes/next/select/close)       |

## Known POC limitations

-   No `ctx.player` / `ctx.world` etc — only the four dialog ops.
-   Player disconnect during a TS dialog leaks the `DialogSession`
    (the pending promise is never resolved). A `pc_logout` hook should
    clear the session.
-   No protection against re-entrant click during an active dialog.
    rAthena's legacy `sd->npc_id != 0` check at the top of `npc_click`
    covers this — we set `sd->npc_id` from `dispatch_npc_click`.
-   `ScriptHost::shutdown` doesn't tear down the V8 platform — V8's
    init is one-shot per process. This is fine for a real server
    (shutdown == process exit); not fine for unit tests.
-   No equivalent of mmo-csharp's `__invokeHookWithCtx` Proxy fallback.
    Calls to unknown `ctx.foo()` from the script will throw at runtime
    instead of silently no-op'ing. For the four wired methods this is
    actually nicer; for a richer API we'd want the Proxy shim back.

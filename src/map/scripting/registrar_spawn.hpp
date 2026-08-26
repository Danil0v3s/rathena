// Installs `registerSpawn()` and `registerFloatingNpc()` on a fresh V8
// context. Mirrors registrar_bindings.hpp / registrar_bindings.cpp, but
// keeps its own file-local storage instead of writing into NpcRegistry,
// since neither registration shape is "an NPC placed on a map" the way
// NpcRegistration is.

#pragma once

#ifdef HAVE_TS_SCRIPTING

#include <v8.h>

namespace rathena::scripting {

	// Bind `globalThis.registerSpawn(...)` and
	// `globalThis.registerFloatingNpc(...)` into `context`. Both accept
	// varargs, mirroring registerNpc().
	//
	// registerSpawn() only *queues* mob spawn points at bundle-eval time;
	// bad JS shapes (missing/mistyped fields) throw immediately, but
	// world-dependent problems (unknown map, unknown mob id, out-of-range
	// coordinates) are deferred to spawn_registered_spawns(), which is
	// expected to run after do_init_npc()/mob_db load, mirroring how
	// npc_spawner.cpp defers map-load checks past bind time.
	//
	// registerFloatingNpc() has no world position — it exists purely to own
	// event labels. `onInit` / `onPCLogin` / `onPCDeath` / each `onTimer`
	// entry / each `onClock` entry are all registered into
	// global_npc_registry() as "<name>::OnInit", "<name>::OnPCLoginEvent",
	// "<name>::OnPCDieEvent", "<name>::OnTimer<ms>" and
	// "<name>::OnClock<HHMM>" respectively (the exact label conventions
	// rAthena's own npc.cpp / script.cpp use for these hooks). IMPORTANT:
	// registering a handler under one of these labels does NOT make it
	// fire — nothing in this translation unit schedules OnTimer/OnClock
	// ticks or hooks PC login/death. See registrar_spawn.cpp for the full
	// list of what's wired vs. not.
	void bind_spawn_registrars(v8::Isolate* isolate, v8::Local<v8::Context> context);

	// Materialize every queued registerSpawn() entry into the running
	// world via npc_parse_mob2() (or the dynamic per-map mob list),
	// mirroring npc_parse_mob() in npc.cpp exactly. Intended to run once
	// per boot, after do_init_npc() so mob_db / map data are ready — same
	// timing as npc_spawner.cpp's spawn_registered_npcs().
	//
	// Returns the number of spawn *points* successfully materialized (each
	// point may represent more than one individual mob, per `amount`).
	int spawn_registered_spawns();

	// Number of registerFloatingNpc() entries currently registered. The
	// parent integration can walk these (once an accessor for the names
	// exists) to fire "<name>::OnInit" at boot, the same way legacy NPCs'
	// OnInit fires after do_init_npc().
	int registered_floating_npc_count();

	// Clear all queued registerSpawn()/registerFloatingNpc() registrations.
	// Intended for tests / @reloadscript-style flows.
	void clear_spawn_registrations();

} // namespace rathena::scripting

#endif // HAVE_TS_SCRIPTING

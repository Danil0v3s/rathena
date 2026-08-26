// Installs the host-side `registerWarp()` and `registerMapFlag()` globals
// on a fresh V8 context, and materializes the resulting registrations into
// the running world once maps are loaded.
//
// Mirrors the structure of registrar_bindings.cpp / npc_registry.hpp /
// npc_spawner.cpp — see those for the established pattern this file
// follows (read_* validation, *_callback varargs dispatch, file-local
// storage, spawn_* materialization called after do_init_npc()).

#pragma once

#ifdef HAVE_TS_SCRIPTING

#include <v8.h>

namespace rathena::scripting {

	// Bind `globalThis.registerWarp(...)` and `globalThis.registerMapFlag(...)`
	// into `context`. Both accept varargs; registrations are stashed in
	// file-local storage inside registrar_warp.cpp until spawn_registered_warps()
	// / apply_registered_mapflags() materialize them.
	void bind_warp_registrars(v8::Isolate* isolate, v8::Local<v8::Context> context);

	// Called after maps are loaded (same point spawn_registered_npcs() runs).
	// Creates a warp NPC for each registerWarp() call whose from/to maps are
	// loaded. Returns the number of warps successfully created.
	int spawn_registered_warps();

	// Applies every registerMapFlag() call whose map is loaded. Returns the
	// number of mapflags successfully applied.
	int apply_registered_mapflags();

	// Clears all pending registration bookkeeping. Called on bundle reload /
	// shutdown so a fresh bundle load starts from empty state.
	void clear_warp_registrations();

} // namespace rathena::scripting

#endif // HAVE_TS_SCRIPTING

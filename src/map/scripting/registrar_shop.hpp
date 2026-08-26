// Installs the host-side `registerShop()` global on a fresh V8 context,
// and materializes the resulting registrations into the running world
// once maps and the item database are loaded.
//
// Mirrors the structure of registrar_warp.cpp — see that file (and
// registrar_bindings.cpp / npc_registry.hpp / npc_spawner.cpp) for the
// established pattern this follows: read_* validation, *_callback
// varargs dispatch, file-local storage, spawn_* materialization called
// after do_init_npc().

#pragma once

#ifdef HAVE_TS_SCRIPTING

#include <v8.h>

namespace rathena::scripting {

	// Bind `globalThis.registerShop(...registrations)` into `context`.
	// Accepts varargs; each argument must match the TS `ShopRegistration`
	// discriminated union (see npc-ts/types/registrations.d.ts). Registrations
	// are stashed in file-local storage inside registrar_shop.cpp until
	// spawn_registered_shops() materializes them.
	void bind_shop_registrar(v8::Isolate* isolate, v8::Local<v8::Context> context);

	// Called after maps and the item database are loaded (same point
	// spawn_registered_npcs() / spawn_registered_warps() run). Creates a
	// shop NPC for each registerShop() call whose map is loaded and that
	// has at least one item with a known item_db id. Returns the number
	// of shops successfully created.
	int spawn_registered_shops();

	// Clears all pending registration bookkeeping. Called on bundle reload /
	// shutdown so a fresh bundle load starts from empty state.
	void clear_shop_registrations();

} // namespace rathena::scripting

#endif // HAVE_TS_SCRIPTING

// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "party_service_custom.hpp"

#if PARTY_SERVICE_EXAMPLES
// Required headers for the example implementations
#include <common/showmsg.hpp>

#include "../../map.hpp"  // For map_mapname2mapid, map_getmapdata
#include "../../pc.hpp"   // For map_session_data access

// ============================================================================
// Configuration for Example Implementations
// ============================================================================

namespace {
	// Minimum base level required to create a party (0 = no restriction)
	constexpr int32 PARTY_CREATE_MIN_LEVEL = 10;

	// Maps where party creation is blocked (tutorial areas, etc.)
	// Add map names as needed
	const char* blocked_maps[] = {
		"new_1-1",   // Tutorial map 1
		"new_1-2",   // Tutorial map 2
		"new_1-3",   // Tutorial map 3
		"new_1-4",   // Tutorial map 4
		"new_2-1",   // Tutorial map
		"new_2-2",   // Tutorial map
		"new_2-3",   // Tutorial map
		"new_2-4",   // Tutorial map
		"new_3-1",   // Tutorial map
		"new_3-2",   // Tutorial map
		"new_3-3",   // Tutorial map
		"new_3-4",   // Tutorial map
		"new_4-1",   // Tutorial map
		"new_4-2",   // Tutorial map
		"new_4-3",   // Tutorial map
		"new_4-4",   // Tutorial map
		"new_5-1",   // Tutorial map
		"new_5-2",   // Tutorial map
		"new_5-3",   // Tutorial map
		"new_5-4",   // Tutorial map
		nullptr      // Sentinel - must be last
	};

	/**
	 * Check if a map is in the blocked list.
	 */
	bool is_map_blocked(int16 map_id) {
		if (map_id < 0)
			return false;

		map_data* mapdata = map_getmapdata(map_id);
		if (mapdata == nullptr)
			return false;

		for (int32 i = 0; blocked_maps[i] != nullptr; i++) {
			if (strcmp(mapdata->name, blocked_maps[i]) == 0) {
				return true;
			}
		}
		return false;
	}
}

// ============================================================================
// Custom Party Service Implementations
// ============================================================================

/**
 * Custom party creation validation.
 *
 * Adds the following checks on top of default validation:
 * 1. Minimum base level requirement (configurable above)
 * 2. Block party creation in tutorial maps
 *
 * These are EXAMPLES - modify or remove as needed for your server.
 */
bool PartyServiceCustom::canCreate(map_session_data& sd, const char* name, int32 item, int32 item2) const {
	// First, run default validation (name check, already-in-party check)
	if (!PartyService::canCreate(sd, name, item, item2)) {
		return false;
	}

	// Custom Check 1: Minimum level requirement
	if (PARTY_CREATE_MIN_LEVEL > 0 && sd.status.base_level < PARTY_CREATE_MIN_LEVEL) {
		// Note: The caller (party_create) handles sending error messages
		// We just return false to deny the action
		ShowDebug("[PartyService] Player %s (Lv.%d) denied party creation: below min level %d\n",
			sd.status.name, sd.status.base_level, PARTY_CREATE_MIN_LEVEL);
		return false;
	}

	// Custom Check 2: Block creation in tutorial maps
	if (is_map_blocked(sd.bl.m)) {
		ShowDebug("[PartyService] Player %s denied party creation: blocked map\n",
			sd.status.name);
		return false;
	}

	// All checks passed
	return true;
}

/**
 * Log party creation events.
 *
 * Called after a party is successfully created.
 * Useful for analytics, debugging, or triggering external systems.
 */
void PartyServiceCustom::onCreated(party_data& party, map_session_data& creator) {
	ShowInfo("[PartyService] Party '%s' (ID: %d) created by %s [Account: %u, Char: %u, Map: %s]\n",
		party.party.name,
		party.party.party_id,
		creator.status.name,
		creator.status.account_id,
		creator.status.char_id,
		map_getmapdata(creator.bl.m) ? map_getmapdata(creator.bl.m)->name : "unknown"
	);
}

#endif // PARTY_SERVICE_EXAMPLES


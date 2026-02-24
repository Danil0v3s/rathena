// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "party_service.hpp"

#include "../../pc.hpp"  // map_session_data definition

// ============================================================================
// Default Implementations
// ============================================================================
// Override in PartyServiceCustom for custom behavior.

bool PartyService::canCreate(map_session_data& sd, const char* name, int32 item, int32 item2) const {
	// empty name
	if (!name[0]) {
		return false;
	}

	// already associated with a party
	if (sd.status.party_id > 0 || sd.party_joining || sd.party_creating) {
		return false;
	}

	return true;
}

bool PartyService::canInvite(map_session_data& sd, map_session_data& target) const {
	// [STUB] Default: allow (actual validation still in party_invite)
	// TODO: Delegate to party_internal::check_invite once extracted
	return true;
}

bool PartyService::canLeave(map_session_data& sd) const {
	// [STUB] Default: allow (actual validation still in party_leave)
	// TODO: Delegate to party_internal::check_leave once extracted
	return true;
}

void PartyService::modifyExpShare(
	party_data& party,
	block_list& source,
	t_exp& base_exp,
	t_exp& job_exp,
	int32& zeny
) const {
	// [STUB] Default: no modification
	// EXP sharing logic remains in party_exp_share for now
}

uint32 PartyService::selectLootRecipient(
	party_data& party,
	map_session_data& looter,
	const item& dropped_item,
	uint32 first_char_id
) const {
	// [STUB] Default: return 0 to use existing party_share_loot logic
	return 0;
}

// ============================================================================
// Event Hooks - Default implementations (no-op)
// ============================================================================

void PartyService::onCreated(party_data& party, map_session_data& creator) {
	// Default: no action
}

void PartyService::onMemberJoined(party_data& party, map_session_data& member) {
	// Default: no action
}

void PartyService::onMemberLeft(party_data& party, uint32 account_id, uint32 char_id) {
	// Default: no action
}

void PartyService::onBroken(int32 party_id) {
	// Default: no action
}

void PartyService::onLeaderChanged(party_data& party, map_session_data& new_leader) {
	// Default: no action
}


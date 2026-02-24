// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "party_service_custom.hpp"

// Include additional headers as needed for your customizations:
// #include "../../pc.hpp"
// #include "../../battle.hpp"
// #include "../../clif.hpp"
// #include "../../achievement.hpp"

// ============================================================================
// Custom Party Service Implementations
// ============================================================================
// Add your custom method implementations below.
// 
// Remember to:
// 1. Declare the method override in party_service_custom.hpp
// 2. Call PartyService::methodName() for default behavior when needed
//
// Example implementations are provided below (disabled with #if 0).

#if 0
// Example: VIP players bypass party creation level check
bool PartyServiceCustom::canCreate(map_session_data& sd, const char* name, int32 item, int32 item2) const {
	// Custom: VIP players can always create parties
	if (pc_isvip(&sd)) {
		// Still validate the name
		if (name == nullptr || name[0] == '\0') {
			return false;
		}
		return true;
	}
	
	// Non-VIP: use default validation
	return PartyService::canCreate(sd, name, item, item2);
}

// Example: Bonus EXP for full parties during events
void PartyServiceCustom::modifyExpShare(
	party_data& party,
	block_list& source,
	t_exp& base_exp,
	t_exp& job_exp,
	int32& zeny
) const {
	// Count online members
	int32 online_count = 0;
	for (int32 i = 0; i < MAX_PARTY; i++) {
		if (party.data[i].sd != nullptr) {
			online_count++;
		}
	}
	
	// Full party bonus: +10% EXP
	if (online_count >= MAX_PARTY) {
		base_exp = base_exp * 110 / 100;
		job_exp = job_exp * 110 / 100;
	}
}

// Example: Log party creation for analytics
void PartyServiceCustom::onCreated(party_data& party, map_session_data& creator) {
	ShowInfo("[PartyAnalytics] Party '%s' created by %s (Account: %u, Char: %u)\n",
		party.party.name,
		creator.status.name,
		creator.status.account_id,
		creator.status.char_id
	);
}
#endif


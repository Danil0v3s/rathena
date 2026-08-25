// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "../party_service.hpp"

/**
 * Custom party service for server-specific modifications.
 *
 * Override only the methods you want to customize.
 * Call parent implementation (PartyService::methodName) for default behavior.
 *
 * Example customizations included:
 * - Minimum level requirement for party creation
 * - Block party creation in tutorial maps
 * - Log party creation events
 *
 * @see SERVICES.md for full documentation and examples
 *
 * To enable examples:
 * 1. Set PARTY_SERVICE_EXAMPLES to 1 below
 * 2. Rebuild the server
 */

// Set to 1 to enable the example implementations
#define PARTY_SERVICE_EXAMPLES 0

class PartyServiceCustom : public PartyService {
public:
#if PARTY_SERVICE_EXAMPLES
	// ========== Validation Hooks ==========

	/**
	 * Custom party creation validation.
	 * Adds: minimum level check, blocked maps check.
	 */
	bool canCreate(map_session_data& sd, const char* name, int32 item, int32 item2) const override;

	// ========== Event Hooks ==========

	/**
	 * Log party creation for analytics/debugging.
	 */
	void onCreated(party_data& party, map_session_data& creator) override;
#endif

	// ========== Additional Methods to Override ==========
	// Uncomment and implement as needed:

	// bool canInvite(map_session_data& sd, map_session_data& target) const override;
	// bool canLeave(map_session_data& sd) const override;
	// void modifyExpShare(party_data& party, block_list& source,
	//                     t_exp& base_exp, t_exp& job_exp, int32& zeny) const override;
	// uint32 selectLootRecipient(party_data& party, map_session_data& looter,
	//                            const item& dropped_item, uint32 first_char_id) const override;
	// void onMemberJoined(party_data& party, map_session_data& member) override;
	// void onMemberLeft(party_data& party, uint32 account_id, uint32 char_id) override;
	// void onBroken(int32 party_id) override;
	// void onLeaderChanged(party_data& party, map_session_data& new_leader) override;
};


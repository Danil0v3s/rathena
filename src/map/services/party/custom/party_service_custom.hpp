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
 * Example customizations:
 * - VIP players can create parties at lower levels
 * - Custom EXP sharing formulas for events
 * - Restrict party creation in certain maps
 * - Custom loot distribution rules
 * 
 * @see SERVICES.md for full documentation and examples
 * 
 * Usage:
 * 1. Uncomment the method declaration below
 * 2. Implement in party_service_custom.cpp
 * 3. Rebuild the server
 */
class PartyServiceCustom : public PartyService {
public:
	// ========== Validation Hooks ==========
	// Uncomment to override:
	
	// bool canCreate(map_session_data& sd, const char* name, int32 item, int32 item2) const override;
	// bool canInvite(map_session_data& sd, map_session_data& target) const override;
	// bool canLeave(map_session_data& sd) const override;

	// ========== Behavior Modifiers ==========
	// Uncomment to override:
	
	// void modifyExpShare(party_data& party, block_list& source,
	//                     t_exp& base_exp, t_exp& job_exp, int32& zeny) const override;
	// uint32 selectLootRecipient(party_data& party, map_session_data& looter,
	//                            const item& dropped_item, uint32 first_char_id) const override;

	// ========== Event Hooks ==========
	// Uncomment to override:
	
	// void onCreated(party_data& party, map_session_data& creator) override;
	// void onMemberJoined(party_data& party, map_session_data& member) override;
	// void onMemberLeft(party_data& party, uint32 account_id, uint32 char_id) override;
	// void onBroken(int32 party_id) override;
	// void onLeaderChanged(party_data& party, map_session_data& new_leader) override;
};


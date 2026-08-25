// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "service_locator.hpp"

#include "party/party_service.hpp"
#include "party/custom/party_service_custom.hpp"

// ============================================================================
// Service Instances
// ============================================================================
// Services are instantiated as static globals.
// Custom services inherit from default services, so they include all
// default behavior unless explicitly overridden.

namespace {
	// Party service - uses custom implementation which inherits from default
	PartyServiceCustom party_service_instance;
	
	// Future services:
	// GuildServiceCustom guild_service_instance;
	// BattleServiceCustom battle_service_instance;
}

// ============================================================================
// Service Accessors
// ============================================================================

PartyService& ServiceLocator::party() {
	return party_service_instance;
}

// Future services:
// GuildService& ServiceLocator::guild() {
//     return guild_service_instance;
// }
// 
// BattleService& ServiceLocator::battle() {
//     return battle_service_instance;
// }


// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

// Forward declarations
class PartyService;
// class GuildService;  // Future
// class BattleService; // Future

/**
 * Central access point for all game services.
 * 
 * Services are compile-time singletons that provide extension points
 * for customizing game behavior. The ServiceLocator resolves the
 * appropriate service implementation (default or custom).
 * 
 * Usage:
 *   #include "services/service_locator.hpp"
 *   
 *   // Option 1: Direct access
 *   ServiceLocator::party().canCreate(sd, name, item, item2);
 *   
 *   // Option 2: Convenience macro
 *   partyService().canCreate(sd, name, item, item2);
 * 
 * @see SERVICES.md for full documentation
 */
class ServiceLocator {
public:
	/**
	 * Get the party service instance.
	 * Returns PartyServiceCustom which inherits from PartyService.
	 */
	static PartyService& party();

	// Future services:
	// static GuildService& guild();
	// static BattleService& battle();
	// static StatusService& status();
	// static ItemService& item();

private:
	// Prevent instantiation - all methods are static
	ServiceLocator() = delete;
	~ServiceLocator() = delete;
	ServiceLocator(const ServiceLocator&) = delete;
	ServiceLocator& operator=(const ServiceLocator&) = delete;
};

// ============================================================================
// Convenience Macros
// ============================================================================
// These provide shorter syntax for accessing services.
// Use whichever style you prefer.

#define partyService() ServiceLocator::party()
// #define guildService() ServiceLocator::guild()   // Future
// #define battleService() ServiceLocator::battle() // Future


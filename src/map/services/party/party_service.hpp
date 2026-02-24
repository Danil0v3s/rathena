// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include <common/cbasetypes.hpp>
#include <common/mmo.hpp>

// Forward declarations
struct party_data;
struct block_list;
struct item;
class map_session_data;

/**
 * PartyService provides extension points for party-related behaviors.
 * 
 * Default implementations preserve existing behavior by delegating to
 * internal functions in party.cpp.
 * 
 * Override specific methods in PartyServiceCustom for customization.
 * 
 * Migration Status Legend:
 *   [STUB]     - Delegates to original function, not yet migrated
 *   [PARTIAL]  - Some logic migrated, some still delegates
 *   [COMPLETE] - Fully migrated, original function deprecated
 * 
 * @see SERVICES.md for full documentation
 */
class PartyService {
public:
	virtual ~PartyService() = default;

	// ========== Validation Hooks ==========
	// Return true to allow action, false to deny.
	// Called before the action is executed.
	// Should NOT send client messages - caller handles that.

	/**
	 * [MIGRATED] Check if player can create a party with given name.
	 * Called before party creation request is sent to char-server.
	 *
	 * Delegates to party_internal::check_create() which validates:
	 * - Party name is not empty
	 * - Player is not already in a party/joining/creating
	 *
	 * @param sd      Player attempting to create party
	 * @param name    Proposed party name (should be trimmed)
	 * @param item    Item sharing option (reserved for future use)
	 * @param item2   Item sharing option 2 (reserved for future use)
	 * @return true if creation should proceed, false to deny
	 */
	virtual bool canCreate(map_session_data& sd, const char* name, int32 item, int32 item2) const;

	/**
	 * [STUB] Check if player can invite target to their party.
	 * 
	 * @param sd      Player sending invitation
	 * @param target  Player being invited
	 * @return true if invitation should proceed, false to deny
	 */
	virtual bool canInvite(map_session_data& sd, map_session_data& target) const;

	/**
	 * [STUB] Check if player can leave their current party.
	 * 
	 * @param sd  Player attempting to leave
	 * @return true if leave should proceed, false to deny
	 */
	virtual bool canLeave(map_session_data& sd) const;

	// ========== Behavior Modifiers ==========
	// Called to customize core game behaviors.
	// Modify parameters in-place.

	/**
	 * [STUB] Modify EXP/Zeny distribution among party members.
	 * Called during party_exp_share before distribution.
	 * 
	 * @param party    The party receiving EXP
	 * @param source   The source of EXP (monster killed, etc)
	 * @param base_exp In/Out - base EXP to distribute
	 * @param job_exp  In/Out - job EXP to distribute
	 * @param zeny     In/Out - zeny to distribute
	 */
	virtual void modifyExpShare(
		party_data& party,
		block_list& source,
		t_exp& base_exp,
		t_exp& job_exp,
		int32& zeny
	) const;

	/**
	 * [STUB] Determine who receives a dropped item.
	 * Called during party_share_loot.
	 * 
	 * @param party         The party
	 * @param looter        Player who picked up the item
	 * @param dropped_item  The item being distributed
	 * @param first_char_id Original looter's char_id
	 * @return char_id of recipient, or 0 to use default behavior
	 */
	virtual uint32 selectLootRecipient(
		party_data& party,
		map_session_data& looter,
		const item& dropped_item,
		uint32 first_char_id
	) const;

	// ========== Event Hooks ==========
	// Called after actions complete successfully.
	// For side effects only: logging, achievements, notifications.
	// Default implementations are empty (no-op).

	/**
	 * Called after a party is successfully created.
	 * @param party   The newly created party
	 * @param creator The player who created the party
	 */
	virtual void onCreated(party_data& party, map_session_data& creator);

	/**
	 * Called after a member successfully joins a party.
	 * @param party  The party joined
	 * @param member The player who joined
	 */
	virtual void onMemberJoined(party_data& party, map_session_data& member);

	/**
	 * Called after a member leaves or is removed from a party.
	 * @param party      The party left
	 * @param account_id Account ID of the member who left
	 * @param char_id    Character ID of the member who left
	 */
	virtual void onMemberLeft(party_data& party, uint32 account_id, uint32 char_id);

	/**
	 * Called after a party is disbanded/broken.
	 * @param party_id ID of the party that was broken
	 */
	virtual void onBroken(int32 party_id);

	/**
	 * Called after party leadership changes.
	 * @param party      The party
	 * @param new_leader The new party leader
	 */
	virtual void onLeaderChanged(party_data& party, map_session_data& new_leader);
};


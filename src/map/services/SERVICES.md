# Service Abstraction Layer

This document describes the service abstraction pattern used in rAthena's map server to enable compile-time customization of game behaviors without modifying core code.

## Overview

The service layer provides **extension points** where server operators can customize specific behaviors by overriding methods in `*ServiceCustom` classes. This follows the same pattern established by the skill system (`SkillImpl` / `SkillFactory`).

### Key Principles

1. **Compile-time customization** - No runtime plugin loading; custom code is compiled with the server
2. **Gradual migration** - Services can be adopted one method at a time
3. **Easy rollback** - Reverting a method is a single-file change
4. **Preserve existing config** - `battle_config` and YAML settings continue to work
5. **Backwards compatible** - Existing free functions (`party_create`, etc.) remain as public API

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                     Custom Layer (Your Code)                        │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │ PartyServiceCustom : PartyService                            │   │
│  │   - Override only methods you want to customize              │   │
│  │   - Call parent for default behavior                         │   │
│  └─────────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────┘
                                  │ inherits
┌─────────────────────────────────▼───────────────────────────────────┐
│                     Service Layer (Default Implementations)         │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │ PartyService                                                 │   │
│  │   - canCreate(), canInvite(), canLeave()                     │   │
│  │   - modifyExpShare(), selectLootRecipient()                  │   │
│  │   - onCreated(), onMemberJoined(), onMemberLeft()            │   │
│  └─────────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────┘
                                  │ delegates to
┌─────────────────────────────────▼───────────────────────────────────┐
│                     Core Layer (Existing Code)                      │
│         party.cpp, guild.cpp, battle.cpp, etc.                     │
│         (Gradually migrated into services)                          │
└─────────────────────────────────────────────────────────────────────┘
```

## Directory Structure

```
src/map/services/
├── SERVICES.md                    # This file
├── service_locator.hpp            # Central access point (ServiceLocator singleton)
├── service_locator.cpp
├── party/
│   ├── party_service.hpp          # Service interface with default implementations
│   ├── party_service.cpp
│   └── custom/
│       ├── party_service_custom.hpp   # Your customizations go here
│       └── party_service_custom.cpp
├── guild/                         # Future
│   ├── guild_service.hpp
│   ├── guild_service.cpp
│   └── custom/
│       └── ...
└── battle/                        # Future
    └── ...
```

## Method Types

Services expose three types of methods:

### 1. Validation Methods (`can*`)

Return `true` to allow an action, `false` to deny.

```cpp
virtual bool canCreate(map_session_data& sd, const char* name, int item, int item2) const;
virtual bool canInvite(map_session_data& sd, map_session_data& target) const;
virtual bool canLeave(map_session_data& sd) const;
```

**Use cases:**
- Add VIP-only features
- Restrict actions in certain maps
- Add custom level/item requirements

### 2. Behavior Modifiers (`modify*`, `select*`)

Modify values or select outcomes for core game logic.

```cpp
virtual void modifyExpShare(party_data& party, block_list& source,
                            t_exp& base_exp, t_exp& job_exp, int32& zeny) const;
virtual uint32 selectLootRecipient(party_data& party, map_session_data& looter,
                                   const item& dropped_item, uint32 first_char_id) const;
```

**Use cases:**
- Custom EXP formulas
- Special loot distribution rules
- Damage modifiers

### 3. Event Hooks (`on*`)

Called after actions complete. For side effects only (logging, achievements, notifications).

```cpp
virtual void onCreated(party_data& party, map_session_data& creator) {}
virtual void onMemberJoined(party_data& party, map_session_data& member) {}
virtual void onMemberLeft(party_data& party, uint32 account_id, uint32 char_id) {}
```

**Use cases:**
- Achievement tracking
- Custom logging
- Discord/external notifications
- Statistics collection

## Accessing Services

Use the `ServiceLocator` singleton:

```cpp
#include "services/service_locator.hpp"

// Option 1: Direct access
ServiceLocator::instance().party().canCreate(sd, name, item, item2);

// Option 2: Convenience macro
partyService().canCreate(sd, name, item, item2);
```

## Customizing a Service

### Step 1: Locate the Custom File

Each service has a `custom/` subdirectory with `*_service_custom.hpp` and `*_service_custom.cpp`.

### Step 2: Override Methods

```cpp
// src/map/services/party/custom/party_service_custom.cpp
#include "party_service_custom.hpp"
#include "../../pc.hpp"  // for pc_isvip, etc.

bool PartyServiceCustom::canCreate(map_session_data& sd, const char* name,
                                    int item, int item2) const {
    // Custom: VIP players can create parties at any level
    if (pc_isvip(&sd)) {
        // Still check name validity
        if (!name || !name[0]) {
            return false;
        }
        return true;
    }

    // Non-VIP: use default behavior
    return PartyService::canCreate(sd, name, item, item2);
}

void PartyServiceCustom::onCreated(party_data& party, map_session_data& creator) {
    // Custom: Log party creation with timestamp
    ShowInfo("[PARTY] %s created party '%s' at %s\n",
             creator.status.name, party.party.name, timestamp());

    // Custom: Grant achievement
    achievement_update_objective(&creator, AG_PARTY_CREATE, 1);
}
```

### Step 3: Declare Overrides in Header

```cpp
// src/map/services/party/custom/party_service_custom.hpp
#pragma once
#include "../party_service.hpp"

class PartyServiceCustom : public PartyService {
public:
    bool canCreate(map_session_data& sd, const char* name,
                   int item, int item2) const override;

    void onCreated(party_data& party, map_session_data& creator) override;
};
```

### Step 4: Rebuild

```bash
make clean && make server
```

## Migration Status Tracking

Each service method is in one of three states:

| State | Description | Service Method | Rollback |
|-------|-------------|----------------|----------|
| **STUB** | Delegates to original code in `*_internal` namespace | Calls internal function | Remove service call from caller |
| **PARTIAL** | Some logic migrated, some delegates | Mixed | Revert to full delegation |
| **COMPLETE** | All logic in service | Self-contained | Revert to STUB |

Methods are marked with their status in comments:

```cpp
/**
 * [STUB] Check if player can create a party.
 * Delegates to party_internal::check_create()
 */
virtual bool canCreate(...) const;

/**
 * [COMPLETE] Modify EXP distribution.
 * Fully migrated from party_exp_share().
 */
virtual void modifyExpShare(...) const;
```

## Adding a New Service

### Step 1: Create Directory Structure

```bash
mkdir -p src/map/services/guild/custom
```

### Step 2: Create Service Interface

```cpp
// src/map/services/guild/guild_service.hpp
#pragma once

class map_session_data;
class MapGuild;

class GuildService {
public:
    virtual ~GuildService() = default;

    // Validation
    virtual bool canCreate(map_session_data& sd, const char* name) const;
    virtual bool canInvite(map_session_data& sd, map_session_data& target) const;

    // Behavior
    virtual void modifyExpContribution(map_session_data& sd, t_exp& exp) const;

    // Events
    virtual void onCreated(MapGuild& guild, map_session_data& creator) {}
    virtual void onMemberJoined(MapGuild& guild, map_session_data& member) {}
};
```

### Step 3: Create Default Implementation

```cpp
// src/map/services/guild/guild_service.cpp
#include "guild_service.hpp"

// Forward declare internal functions (in guild.cpp)
namespace guild_internal {
    bool check_create(map_session_data& sd, const char* name);
    bool check_invite(map_session_data& sd, map_session_data& target);
}

bool GuildService::canCreate(map_session_data& sd, const char* name) const {
    // [STUB] Delegate to existing logic
    return guild_internal::check_create(sd, name);
}

bool GuildService::canInvite(map_session_data& sd, map_session_data& target) const {
    // [STUB] Delegate to existing logic
    return guild_internal::check_invite(sd, target);
}

void GuildService::modifyExpContribution(map_session_data& sd, t_exp& exp) const {
    // [STUB] No modification - uses existing battle_config settings
}
```

### Step 4: Create Custom Service

```cpp
// src/map/services/guild/custom/guild_service_custom.hpp
#pragma once
#include "../guild_service.hpp"

class GuildServiceCustom : public GuildService {
    // Override methods as needed
};
```

```cpp
// src/map/services/guild/custom/guild_service_custom.cpp
#include "guild_service_custom.hpp"

// Add custom overrides here
```

### Step 5: Register in ServiceLocator

```cpp
// src/map/services/service_locator.hpp
class ServiceLocator : public rathena::util::Singleton<ServiceLocator> {
public:
    PartyService& party();
    GuildService& guild();  // Add new service
    // ...
};

#define guildService() ServiceLocator::instance().guild()
```

```cpp
// src/map/services/service_locator.cpp
#include "guild/guild_service.hpp"
#include "guild/custom/guild_service_custom.hpp"

namespace {
    PartyServiceCustom party_service_instance;
    GuildServiceCustom guild_service_instance;  // Add instance
}

GuildService& ServiceLocator::guild() {
    return guild_service_instance;
}
```

### Step 6: Wire Into Existing Code

```cpp
// guild.cpp
#include "services/service_locator.hpp"

bool guild_create(map_session_data& sd, const char* name) {
    // Add service hook
    if (!guildService().canCreate(sd, name)) {
        clif_guild_created(sd, 1);  // Failed
        return false;
    }

    // ... existing logic ...

    // Add event hook at end
    guildService().onCreated(*g, sd);
    return true;
}
```

## Migrating a Method (Step by Step)

This section shows how to migrate logic from existing code into a service method.

### Example: Migrating `canCreate` for Party

#### Phase 1: STUB (No behavior change)

**1a. Extract validation to internal namespace (party.cpp)**

```cpp
// party.cpp - near top of file
namespace party_internal {

bool check_create(map_session_data& sd, const char* name, int item, int item2) {
    // Move existing validation from party_create() here
    char tname[NAME_LENGTH];
    safestrncpy(tname, name, NAME_LENGTH);
    trim(tname);

    if (!tname[0]) {
        clif_party_created(sd, 1);  // Empty name
        return false;
    }
    if (sd.status.party_id > 0) {
        clif_party_created(sd, 2);  // Already in party
        return false;
    }
    if (battle_config.basic_skill_check &&
        pc_checkskill(&sd, NV_BASIC) < 7 &&
        !party_create_byscript) {
        clif_party_created(sd, 1);  // Skill check
        return false;
    }

    return true;
}

} // namespace party_internal
```

**1b. Update party_create to use internal function**

```cpp
// party.cpp
int32 party_create(map_session_data& sd, char* name, int item, int item2) {
    if (!party_internal::check_create(sd, name, item, item2)) {
        return 0;  // Already sent error message
    }

    // ... rest of function unchanged ...
}
```

**1c. Service delegates to internal**

```cpp
// party_service.cpp
namespace party_internal {
    bool check_create(map_session_data& sd, const char* name, int item, int item2);
}

bool PartyService::canCreate(map_session_data& sd, const char* name,
                              int item, int item2) const {
    // [STUB] Pure delegation - no behavior change
    return party_internal::check_create(sd, name, item, item2);
}
```

#### Phase 2: Wire Service Into Caller

```cpp
// party.cpp
#include "services/service_locator.hpp"

int32 party_create(map_session_data& sd, char* name, int item, int item2) {
    // Now uses service (which still delegates to internal)
    if (!partyService().canCreate(sd, name, item, item2)) {
        return 0;
    }
    // ... rest unchanged ...
}
```

At this point, behavior is **identical** to before. Custom services can now override.

#### Phase 3: Migrate Logic Into Service (Optional)

Only do this when you want the service to be self-contained.

```cpp
// party_service.cpp
bool PartyService::canCreate(map_session_data& sd, const char* name,
                              int item, int item2) const {
    // [COMPLETE] - Logic moved from party_internal
    char tname[NAME_LENGTH];
    safestrncpy(tname, name, NAME_LENGTH);
    trim(tname);

    if (!tname[0]) return false;
    if (sd.status.party_id > 0) return false;
    if (battle_config.basic_skill_check &&
        pc_checkskill(&sd, NV_BASIC) < 7 &&
        !party_create_byscript) {
        return false;
    }

    return true;
}
```

Note: Error messages (`clif_party_created`) should be sent by the caller, not the service.

## Rollback Procedures

### Rollback a Single Method

To rollback a migrated method to STUB state:

```cpp
// party_service.cpp
bool PartyService::canCreate(...) const {
    // Revert to delegation
    return party_internal::check_create(sd, name, item, item2);
}
```

### Rollback Service Integration

To completely remove service from a function:

```cpp
// party.cpp - Before (with service)
int32 party_create(map_session_data& sd, char* name, int item, int item2) {
    if (!partyService().canCreate(sd, name, item, item2)) {
        return 0;
    }
    // ...
}

// party.cpp - After (rollback)
int32 party_create(map_session_data& sd, char* name, int item, int item2) {
    if (!party_internal::check_create(sd, name, item, item2)) {
        return 0;
    }
    // ...
}
```

## Best Practices

### DO

- **Call parent implementation** when you only want to add behavior:
  ```cpp
  bool PartyServiceCustom::canCreate(...) const {
      if (!PartyService::canCreate(sd, name, item, item2)) {
          return false;  // Default checks failed
      }
      // Add your custom checks
      return myCustomCheck(sd);
  }
  ```

- **Keep services stateless** - Don't store mutable state in services
- **Use `const` methods** - Service methods should not modify the service itself
- **Respect battle_config** - Check existing config settings before adding new behavior
- **Document migration status** - Mark methods as `[STUB]`, `[PARTIAL]`, or `[COMPLETE]`

### DON'T

- **Don't send client messages from validation methods** - Let the caller handle UI
- **Don't modify game state in `can*` methods** - They should be pure checks
- **Don't skip parent implementation** unless you're completely replacing behavior
- **Don't add new config settings in services** - Use existing `battle_config` or propose a YAML change

## Services Roadmap

| Service | Status | Priority | Complexity |
|---------|--------|----------|------------|
| PartyService | Planned | 1 (Start) | Low |
| GuildService | Planned | 2 | Medium |
| BattleService | Planned | 3 | High |
| StatusService | Future | 4 | Medium |
| ItemService | Future | 5 | Medium |
| InstanceService | Future | 6 | Low |

## FAQ

### Why compile-time instead of runtime plugins?

- **Performance** - No virtual dispatch overhead for hot paths
- **Safety** - Compiler catches type errors
- **Simplicity** - No plugin loading/unloading complexity
- **Debugging** - Standard debugging tools work

### How is this different from the skill system?

The skill system creates **new instances** per skill ID. Services are **singletons** that modify behavior through virtual method dispatch. Both use inheritance for customization.

### Can I have multiple custom implementations?

No. There's one `*ServiceCustom` class per service. If you need conditional behavior, implement the branching logic inside your custom methods.

### What about thread safety?

Services are stateless singletons accessed from the main thread only. No synchronization needed for current rAthena architecture.

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

| State | Description | Rollback |
|-------|-------------|----------|
| **STUB** | Returns permissive default (e.g., `return true`) | Remove service call from caller |
| **MIGRATED** | Contains actual validation/logic | Inline logic back into caller |

Example documentation:

```cpp
/**
 * Check if player can create a party.
 *
 * Validates:
 * - Party name is not empty
 * - Player is not already in a party/joining/creating
 */
virtual bool canCreate(...) const;
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

bool GuildService::canCreate(map_session_data& sd, const char* name) const {
    // Validate name
    if (name == nullptr || name[0] == '\0') {
        return false;
    }

    // Check if already in a guild
    if (sd.status.guild_id > 0) {
        return false;
    }

    return true;
}

bool GuildService::canInvite(map_session_data& sd, map_session_data& target) const {
    // Check if inviter is in a guild with invite permissions
    if (sd.status.guild_id == 0) {
        return false;
    }

    // Check if target is already in a guild
    if (target.status.guild_id > 0) {
        return false;
    }

    return true;
}

void GuildService::modifyExpContribution(map_session_data& sd, t_exp& exp) const {
    // Default: no modification - uses existing battle_config settings
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

#### Step 1: Implement validation in the service

Move the validation logic directly into `PartyService::canCreate()`:

```cpp
// party_service.cpp
bool PartyService::canCreate(map_session_data& sd, const char* name,
                              int32 item, int32 item2) const {
    // Validate name is not empty (caller should trim before calling)
    if (name == nullptr || name[0] == '\0') {
        return false;
    }

    // Check if already associated with a party
    if (sd.status.party_id > 0 || sd.party_joining || sd.party_creating) {
        return false;
    }

    return true;
}
```

#### Step 2: Wire service into the caller

Update the original function to call the service:

```cpp
// party.cpp
#include "services/service_locator.hpp"

int32 party_create(map_session_data& sd, char* name, int item, int item2) {
    char tname[NAME_LENGTH];
    safestrncpy(tname, name, NAME_LENGTH);
    trim(tname);

    // Service validation hook - allows customization
    if (!partyService().canCreate(sd, tname, item, item2)) {
        // Determine failure reason for client message
        if (!tname[0]) {
            return 0;  // empty name - silent fail
        }
        // already in party/joining/creating
        clif_party_created(sd, 2);
        return -2;
    }

    sd.party_creating = true;
    party_fill_member(leader, sd, 1);
    intif_create_party(&leader, name, item, item2);
    return 1;
}
```

**Key points:**
- The service method returns `bool` only - no side effects
- The caller handles error messages (`clif_party_created`)
- The caller prepares data (trimming name) before calling the service

#### Step 3: Customize (optional)

Override in `PartyServiceCustom` for server-specific behavior:

```cpp
// party_service_custom.cpp
bool PartyServiceCustom::canCreate(map_session_data& sd, const char* name,
                                    int32 item, int32 item2) const {
    // Run default validation first
    if (!PartyService::canCreate(sd, name, item, item2)) {
        return false;
    }

    // Custom: minimum level requirement
    if (sd.status.base_level < 10) {
        return false;
    }

    return true;
}
```

## Rollback Procedures

### Rollback to Original Code

To completely remove service integration from a function, inline the validation:

```cpp
// party.cpp - Before (with service)
int32 party_create(map_session_data& sd, char* name, int item, int item2) {
    // ... trim name ...
    if (!partyService().canCreate(sd, tname, item, item2)) {
        // handle error
    }
    // ...
}

// party.cpp - After (rollback)
int32 party_create(map_session_data& sd, char* name, int item, int item2) {
    // ... trim name ...
    if (!tname[0]) {
        return 0;
    }
    if (sd.status.party_id > 0 || sd.party_joining || sd.party_creating) {
        clif_party_created(sd, 2);
        return -2;
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
- **Document what the method validates** - List the checks performed in the docstring

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

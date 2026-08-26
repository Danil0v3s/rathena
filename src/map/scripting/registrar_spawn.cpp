#ifdef HAVE_TS_SCRIPTING

#include "registrar_spawn.hpp"

#include <cstring>
#include <string>
#include <vector>

#include "js_object_reader.hpp"
#include "npc_registry.hpp"
#include "../battle.hpp"
#include "../map.hpp"
#include "../mob.hpp"
#include "../npc.hpp"
#include "../../common/cbasetypes.hpp"
#include "../../common/malloc.hpp"
#include "../../common/showmsg.hpp"
#include "../../common/strlib.hpp"

namespace rathena::scripting {

	namespace {

		// Pull a boolean field off an object. js_object_reader.hpp has no
		// Optional<bool> helper (nothing else needed one yet), so this
		// mirrors js::optional_int's shape exactly: fallback when missing,
		// throw when present but not actually a boolean.
		bool optional_bool(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Object> obj, const char* field, bool fallback) {
			auto key = v8::String::NewFromUtf8(iso, field).ToLocalChecked();
			v8::Local<v8::Value> v;
			if (!obj->Get(ctx, key).ToLocal(&v) || js::is_undefined_or_null(v))
				return fallback;
			if (!v->IsBoolean()) {
				throw ScriptRegistrationError(std::string("field '") + field +
				    "' must be a boolean when set; got " + js::describe_type(v) + ".");
			}
			return v->BooleanValue(iso);
		}

		// ==================================================================
		// registerSpawn
		// ==================================================================

		// JS-shape-validated snapshot of one SpawnRegistration entry. World
		// materialization (map lookup, mob_db lookup, spawn_data
		// construction) happens later in spawn_registered_spawns(), not
		// here — this struct just carries what the bundle handed us.
		struct SpawnEntry {
			std::string map;
			bool has_area = false;
			int x = 0, y = 0, xs = 0, ys = 0; // only meaningful if has_area
			int mob_id = 0;
			int amount = 1;
			uint32_t delay1 = 5000; // respawn.baseMs, ms — matches npc_parse_mob's default
			uint32_t delay2 = 0;    // respawn.jitterMs, ms
			bool boss = false;
			std::string name;     // empty -> mob_db default, resolved at materialize time
			std::string on_death; // full "Label::OnEvent" string, or empty
			int size = 0;         // 0 = mob_db default (matches spawn_data::state.size semantics)
			int ai = 0;           // 0 = AI_NONE / no override
		};

		std::vector<SpawnEntry> g_spawns;

		SpawnEntry read_spawn(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Value> raw, size_t index) {
			if (raw.IsEmpty() || !raw->IsObject() || raw->IsArray() || raw->IsFunction()) {
				throw ScriptRegistrationError(
				    "registerSpawn()[" + std::to_string(index) + "] must be an object; got " +
				    js::describe_type(raw) + ".");
			}
			auto obj = raw.As<v8::Object>();
			std::string trace = "registerSpawn()[" + std::to_string(index) + "]";

			SpawnEntry e;
			e.map = js::require_string(iso, ctx, obj, "map", trace);
			e.mob_id = js::require_int(iso, ctx, obj, "mobId", trace);
			e.amount = js::require_int(iso, ctx, obj, "amount", trace);
			e.name = js::optional_string(iso, ctx, obj, "name", "");
			e.on_death = js::optional_string(iso, ctx, obj, "onDeath", "");
			e.size = js::optional_int(iso, ctx, obj, "size", 0);
			e.ai = js::optional_int(iso, ctx, obj, "ai", 0);
			e.boss = optional_bool(iso, ctx, obj, "boss", false);

			if (auto area_maybe = js::optional_object(iso, ctx, obj, "area"); !area_maybe.IsEmpty()) {
				auto area = area_maybe.ToLocalChecked();
				e.has_area = true;
				e.x = js::require_int(iso, ctx, area, "x", trace + ".area");
				e.y = js::require_int(iso, ctx, area, "y", trace + ".area");
				e.xs = js::require_int(iso, ctx, area, "xs", trace + ".area");
				e.ys = js::require_int(iso, ctx, area, "ys", trace + ".area");
			}

			if (auto respawn_maybe = js::optional_object(iso, ctx, obj, "respawn"); !respawn_maybe.IsEmpty()) {
				auto respawn = respawn_maybe.ToLocalChecked();
				int base = js::require_int(iso, ctx, respawn, "baseMs", trace + ".respawn");
				int jitter = js::optional_int(iso, ctx, respawn, "jitterMs", 0);
				if (base < 0 || jitter < 0) {
					throw ScriptRegistrationError(trace + ".respawn: 'baseMs'/'jitterMs' must be >= 0.");
				}
				e.delay1 = static_cast<uint32_t>(base);
				e.delay2 = static_cast<uint32_t>(jitter);
			}

			return e;
		}

		void register_spawn_callback(const v8::FunctionCallbackInfo<v8::Value>& info) {
			auto* iso = info.GetIsolate();
			auto ctx = iso->GetCurrentContext();
			try {
				for (int i = 0; i < info.Length(); ++i)
					g_spawns.push_back(read_spawn(iso, ctx, info[i], static_cast<size_t>(i)));
			} catch (const std::exception& e) {
				iso->ThrowException(v8::Exception::Error(
				    v8::String::NewFromUtf8(iso, e.what()).ToLocalChecked()));
			}
		}

		// ==================================================================
		// registerFloatingNpc
		// ==================================================================

		std::vector<std::string> g_floating_names;

		// Register every own property of `record` (an onTimer/onClock
		// Record<K, handler>) into the event registry as
		// "<npc_name>::<label_prefix><key>". Key stringification handles
		// both onTimer's numeric keys (e.g. 5000 -> "5000") and onClock's
		// string keys (e.g. "0930") uniformly, since V8 normalizes
		// integer-like own property names to their canonical string form.
		void register_handler_record(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Object> record, const std::string& npc_name,
		    const char* label_prefix, const std::string& trace) {
			auto& events = global_npc_registry();
			auto own_names = record->GetOwnPropertyNames(ctx).ToLocalChecked();
			for (uint32_t i = 0; i < own_names->Length(); ++i) {
				v8::Local<v8::Value> key_v;
				if (!own_names->Get(ctx, i).ToLocal(&key_v))
					continue;
				v8::Local<v8::Value> val_v;
				if (!record->Get(ctx, key_v).ToLocal(&val_v))
					continue;
				if (!val_v->IsFunction()) {
					throw ScriptRegistrationError(trace + "['" + js::to_std_string(iso, key_v) +
					    "'] must be a function; got " + js::describe_type(val_v) + ".");
				}
				std::string label = npc_name + "::" + label_prefix + js::to_std_string(iso, key_v);
				v8::Global<v8::Function> g(iso, val_v.As<v8::Function>());
				events.add_event_handler(label, std::move(g));
			}
		}

		void read_floating_npc(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Value> raw, size_t index) {
			if (raw.IsEmpty() || !raw->IsObject() || raw->IsArray() || raw->IsFunction()) {
				throw ScriptRegistrationError(
				    "registerFloatingNpc()[" + std::to_string(index) + "] must be an object; got " +
				    js::describe_type(raw) + ".");
			}
			auto obj = raw.As<v8::Object>();
			std::string bind_trace = "registerFloatingNpc()[" + std::to_string(index) + "]";
			auto name = js::require_string(iso, ctx, obj, "name", bind_trace);
			std::string trace = "registerFloatingNpc('" + name + "')";

			auto& events = global_npc_registry();

			// Label conventions below match rAthena's own defaults exactly
			// (see Script_Config in script.cpp: init_event_name = "OnInit",
			// login_event_name = "OnPCLoginEvent", die_event_name =
			// "OnPCDieEvent", timer_event_name = "OnTimer",
			// timer_clock_event_name = "OnClock"). Registering here only
			// makes the handler *findable* by label — see the header
			// comment and the note at the bottom of this function for what
			// still needs wiring before any of these actually fire.
			if (auto fn = js::optional_function(iso, ctx, obj, "onInit", trace); !fn.IsEmpty()) {
				events.add_event_handler(name + "::OnInit",
				    v8::Global<v8::Function>(iso, fn.ToLocalChecked()));
			}
			if (auto fn = js::optional_function(iso, ctx, obj, "onPCLogin", trace); !fn.IsEmpty()) {
				events.add_event_handler(name + "::OnPCLoginEvent",
				    v8::Global<v8::Function>(iso, fn.ToLocalChecked()));
			}
			if (auto fn = js::optional_function(iso, ctx, obj, "onPCDeath", trace); !fn.IsEmpty()) {
				events.add_event_handler(name + "::OnPCDieEvent",
				    v8::Global<v8::Function>(iso, fn.ToLocalChecked()));
			}

			if (auto timers = js::optional_object(iso, ctx, obj, "onTimer"); !timers.IsEmpty()) {
				register_handler_record(iso, ctx, timers.ToLocalChecked(), name, "OnTimer",
				    trace + ".onTimer");
			}
			if (auto clocks = js::optional_object(iso, ctx, obj, "onClock"); !clocks.IsEmpty()) {
				register_handler_record(iso, ctx, clocks.ToLocalChecked(), name, "OnClock",
				    trace + ".onClock");
			}

			// NOT YET DISPATCHED: nothing in this translation unit fires
			// OnInit at boot, ticks OnTimer<ms> on a timer, fires
			// OnClock<HHMM> off the wall clock, or hooks OnPCLoginEvent /
			// OnPCDieEvent off the login/death paths. The handlers above
			// are discoverable via global_npc_registry().find_event_handler()
			// (e.g. from ctx.doevent()) but need timer + login/death-hook
			// plumbing elsewhere before they fire on their own. g_floating_names
			// below exists so a future boot sequence can walk these names and
			// fire "<name>::OnInit" once, the way legacy NPCs' OnInit does
			// after do_init_npc().
			g_floating_names.push_back(name);
		}

		void register_floating_npc_callback(const v8::FunctionCallbackInfo<v8::Value>& info) {
			auto* iso = info.GetIsolate();
			auto ctx = iso->GetCurrentContext();
			try {
				for (int i = 0; i < info.Length(); ++i)
					read_floating_npc(iso, ctx, info[i], static_cast<size_t>(i));
			} catch (const std::exception& e) {
				iso->ThrowException(v8::Exception::Error(
				    v8::String::NewFromUtf8(iso, e.what()).ToLocalChecked()));
			}
		}

	} // namespace

	void bind_spawn_registrars(v8::Isolate* isolate, v8::Local<v8::Context> context) {
		auto global = context->Global();

		{
			auto tmpl = v8::FunctionTemplate::New(isolate, &register_spawn_callback);
			auto fn = tmpl->GetFunction(context).ToLocalChecked();
			auto key = v8::String::NewFromUtf8(isolate, "registerSpawn").ToLocalChecked();
			(void)global->Set(context, key, fn);
		}
		{
			auto tmpl = v8::FunctionTemplate::New(isolate, &register_floating_npc_callback);
			auto fn = tmpl->GetFunction(context).ToLocalChecked();
			auto key = v8::String::NewFromUtf8(isolate, "registerFloatingNpc").ToLocalChecked();
			(void)global->Set(context, key, fn);
		}
	}

	int spawn_registered_spawns() {
		int spawned = 0;

		for (const auto& e : g_spawns) {
			int16 mapid = map_mapname2mapid(e.map.c_str());
			if (mapid < 0) {
				ShowWarning("[ts-scripting] cannot spawn mob %d: map '%s' not loaded.\n",
				    e.mob_id, e.map.c_str());
				continue;
			}
			if (mobdb_checkid(e.mob_id) == 0) {
				ShowWarning("[ts-scripting] cannot spawn on '%s': unknown mob id %d.\n",
				    e.map.c_str(), e.mob_id);
				continue;
			}
			if (e.amount < 1 || e.amount > 1000) {
				ShowWarning("[ts-scripting] cannot spawn mob %d on '%s': amount %d out of range [1,1000].\n",
				    e.mob_id, e.map.c_str(), e.amount);
				continue;
			}
			if (e.size < 0 || e.size > SZ_BIG) {
				ShowWarning("[ts-scripting] cannot spawn mob %d on '%s': invalid size %d.\n",
				    e.mob_id, e.map.c_str(), e.size);
				continue;
			}
			if (e.ai < AI_NONE || e.ai >= AI_MAX) {
				ShowWarning("[ts-scripting] cannot spawn mob %d on '%s': invalid ai %d.\n",
				    e.mob_id, e.map.c_str(), e.ai);
				continue;
			}

			struct map_data* mapdata = map_getmapdata(mapid);

			// area omitted <=> {0,0,0,0}: matches npc_parse_mob's own
			// zero-initialized defaults for an unspecified <x>,<y>,<xs>,<ys>.
			int x = e.has_area ? e.x : 0;
			int y = e.has_area ? e.y : 0;
			int xs = e.has_area ? e.xs : 0;
			int ys = e.has_area ? e.ys : 0;

			if (x < 0 || x >= mapdata->xs || y < 0 || y >= mapdata->ys) {
				ShowWarning("[ts-scripting] cannot spawn mob %d on '%s': coordinates (%d,%d) out of range (map is %dx%d).\n",
				    e.mob_id, e.map.c_str(), x, y, mapdata->xs, mapdata->ys);
				continue;
			}

			// Mirrors npc_parse_mob(): a negative span means "map-wide"
			// (warn, then treat as 0); a zero span anchored on a real
			// coordinate means "this exact cell" (bumped to a span of 1,
			// since the free-cell search below subtracts 1 from it).
			if (xs < 0) {
				ShowWarning("[ts-scripting] negative x-span %d for mob %d on '%s'; defaulting to map-wide.\n",
				    xs, e.mob_id, e.map.c_str());
				xs = 0;
			} else if (xs == 0 && x > 0) {
				xs = 1;
			}
			if (ys < 0) {
				ShowWarning("[ts-scripting] negative y-span %d for mob %d on '%s'; defaulting to map-wide.\n",
				    ys, e.mob_id, e.map.c_str());
				ys = 0;
			} else if (ys == 0 && y > 0) {
				ys = 1;
			}

			spawn_data mob{};
			mob.m = static_cast<uint16>(mapid);
			mob.x = static_cast<uint16>(x);
			mob.y = static_cast<uint16>(y);
			mob.xs = static_cast<int16>(xs);
			mob.ys = static_cast<int16>(ys);
			// NOTE: unlike npc_parse_mob, we don't additionally force
			// mob.x/mob.y to the sentinel (uint16)-1 when xs==ys==0 ("spawn
			// anywhere"). mob_spawn() (mob.cpp) branches into the free-cell
			// search path whenever spawn->xs + spawn->ys < 1 regardless of
			// x/y's value, so the sentinel is cosmetic for that case — 0,0
			// behaves identically at runtime.

			mob.id = static_cast<int16>(e.mob_id);
			mob.num = static_cast<uint16>(e.amount);
			mob.active = 0;
			mob.delay1 = e.delay1;
			mob.delay2 = e.delay2;
			mob.state.boss = e.boss ? 1 : 0;
			if (e.size > SZ_SMALL && e.size <= SZ_BIG)
				mob.state.size = e.size;
			if (e.ai > AI_NONE && e.ai < AI_MAX)
				mob.state.ai = static_cast<enum mob_ai>(e.ai);

			std::string name = e.name;
			if (name.empty()) {
				auto db = mob_db.find(e.mob_id);
				if (db)
					name = db->name;
			}
			safestrncpy(mob.name, name.c_str(), sizeof(mob.name));
			safestrncpy(mob.eventname, e.on_death.c_str(), sizeof(mob.eventname));
			safestrncpy(mob.filepath, "ts-scripting::registerSpawn", sizeof(mob.filepath));

			if (!mob_parse_dataset(&mob)) {
				ShowWarning("[ts-scripting] invalid spawn dataset for mob %d on '%s'.\n",
				    e.mob_id, e.map.c_str());
				continue;
			}

			// Update mob_db's spawn lookup table (used by e.g. the "where
			// to find this mob" client info), same as npc_parse_mob.
			struct spawn_info info{ mapdata->index, mob.num };
			mob_add_spawn(static_cast<uint16>(e.mob_id), info);

			// Heap-allocate the actual spawn record. Ownership does NOT
			// stay with us: npc_parse_mob2() creates mob_data instances
			// that keep a raw, non-owning `spawn` pointer back into this
			// block (used for respawns for as long as the map-server
			// runs), and — when battle_config.dynamic_mobs is on — the
			// pointer is also stashed directly in the map's moblist[]
			// slot. We deliberately never free `data`; this exactly
			// mirrors npc_parse_mob's own aMalloc + memcpy, which never
			// frees it either (only the legacy filepath-based unload path
			// does, which registerSpawn() doesn't hook into).
			auto* data = static_cast<spawn_data*>(aMalloc(sizeof(spawn_data)));
			memcpy(data, &mob, sizeof(spawn_data));

			// npc_install_mob_spawn() lives in npc.cpp so it can also bump
			// the file-static npc_mob / npc_cache_mob / npc_delay_mob
			// counters that feed rAthena's boot summary.
			npc_install_mob_spawn(data);

			++spawned;
			ShowStatus("[ts-scripting] spawned %d x mob %d on '%s'.\n",
			    e.amount, e.mob_id, e.map.c_str());
		}

		return spawned;
	}

	int registered_floating_npc_count() {
		return static_cast<int>(g_floating_names.size());
	}

	void clear_spawn_registrations() {
		g_spawns.clear();
		g_floating_names.clear();
	}

} // namespace rathena::scripting

#endif // HAVE_TS_SCRIPTING

#ifdef HAVE_TS_SCRIPTING

#include "registrar_warp.hpp"

#include <cstdio>
#include <string>
#include <vector>

#include "js_object_reader.hpp"
#include "../map.hpp"
#include "../npc.hpp"
#include "../../common/cbasetypes.hpp"
#include "../../common/mapindex.hpp"
#include "../../common/showmsg.hpp"
#include "../../common/strlib.hpp"

namespace rathena::scripting {

	namespace {

		// A pending registerWarp() call, captured verbatim from JS. Resolved
		// into a live warp NPC by spawn_registered_warps() once maps are
		// loaded.
		struct WarpRegistration {
			std::string from_map;
			int from_x{0};
			int from_y{0};
			int xs{0};
			int ys{0};
			std::string to_map;
			int to_x{0};
			int to_y{0};
			bool trigger_on_hidden{false}; // type === "warp2"
		};

		// A pending registerMapFlag() call. `value` mirrors the raw string a
		// legacy npc script would put in w4 — parsed per-flag at apply time,
		// same as npc_parse_mapflag does.
		struct MapFlagRegistration {
			std::string map;
			std::string flag;
			std::string value;
			bool has_value{false};
		};

		// File-local storage — kept out of the header so registrar_warp.hpp
		// stays a clean public surface. Function-local statics give us
		// lazy, well-defined initialization order.
		std::vector<WarpRegistration>& pending_warps() {
			static std::vector<WarpRegistration> instance;
			return instance;
		}

		std::vector<MapFlagRegistration>& pending_mapflags() {
			static std::vector<MapFlagRegistration> instance;
			return instance;
		}

		WarpRegistration read_warp(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Value> raw, size_t index) {
			std::string trace = "registerWarp[" + std::to_string(index) + "]";
			if (raw.IsEmpty() || !raw->IsObject() || raw->IsArray() || raw->IsFunction()) {
				throw ScriptRegistrationError(
				    trace + " must be an object; got " + js::describe_type(raw) + ".");
			}
			auto obj = raw.As<v8::Object>();

			WarpRegistration reg;

			auto from_maybe = js::optional_object(iso, ctx, obj, "from");
			if (from_maybe.IsEmpty()) {
				throw ScriptRegistrationError(trace + ": required field 'from' is missing.");
			}
			auto from = from_maybe.ToLocalChecked();
			reg.from_map = js::require_string(iso, ctx, from, "map", trace + ".from");
			reg.from_x = js::require_int(iso, ctx, from, "x", trace + ".from");
			reg.from_y = js::require_int(iso, ctx, from, "y", trace + ".from");

			auto area_maybe = js::optional_object(iso, ctx, obj, "area");
			if (area_maybe.IsEmpty()) {
				throw ScriptRegistrationError(trace + ": required field 'area' is missing.");
			}
			auto area = area_maybe.ToLocalChecked();
			reg.xs = js::require_int(iso, ctx, area, "xs", trace + ".area");
			reg.ys = js::require_int(iso, ctx, area, "ys", trace + ".area");

			auto to_maybe = js::optional_object(iso, ctx, obj, "to");
			if (to_maybe.IsEmpty()) {
				throw ScriptRegistrationError(trace + ": required field 'to' is missing.");
			}
			auto to = to_maybe.ToLocalChecked();
			reg.to_map = js::require_string(iso, ctx, to, "map", trace + ".to");
			reg.to_x = js::require_int(iso, ctx, to, "x", trace + ".to");
			reg.to_y = js::require_int(iso, ctx, to, "y", trace + ".to");

			std::string type = js::optional_string(iso, ctx, obj, "type", "warp");
			if (type == "warp2") {
				reg.trigger_on_hidden = true;
			} else if (type != "warp") {
				throw ScriptRegistrationError(
				    trace + ": field 'type' must be \"warp\" or \"warp2\"; got \"" + type + "\".");
			}

			return reg;
		}

		MapFlagRegistration read_mapflag(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Value> raw, size_t index) {
			std::string trace = "registerMapFlag[" + std::to_string(index) + "]";
			if (raw.IsEmpty() || !raw->IsObject() || raw->IsArray() || raw->IsFunction()) {
				throw ScriptRegistrationError(
				    trace + " must be an object; got " + js::describe_type(raw) + ".");
			}
			auto obj = raw.As<v8::Object>();

			MapFlagRegistration reg;
			reg.map = js::require_string(iso, ctx, obj, "map", trace);
			reg.flag = js::require_string(iso, ctx, obj, "flag", trace);

			// optional_string() can't tell "missing" apart from "present but
			// empty", and we need that distinction (an omitted value means
			// "enable the flag with its default", a present-but-empty one
			// is a caller mistake we still want to try to honor). Probe the
			// property directly first.
			auto value_key = v8::String::NewFromUtf8(iso, "value").ToLocalChecked();
			v8::Local<v8::Value> value_v;
			if (obj->Get(ctx, value_key).ToLocal(&value_v) && !js::is_undefined_or_null(value_v)) {
				reg.value = js::optional_string(iso, ctx, obj, "value");
				reg.has_value = true;
			}

			return reg;
		}

		void register_warp_callback(const v8::FunctionCallbackInfo<v8::Value>& info) {
			auto* isolate = info.GetIsolate();
			auto ctx = isolate->GetCurrentContext();
			try {
				for (int i = 0; i < info.Length(); ++i) {
					pending_warps().push_back(read_warp(isolate, ctx, info[i], static_cast<size_t>(i)));
				}
			} catch (const std::exception& e) {
				isolate->ThrowException(v8::Exception::Error(
				    v8::String::NewFromUtf8(isolate, e.what()).ToLocalChecked()));
			}
		}

		void register_mapflag_callback(const v8::FunctionCallbackInfo<v8::Value>& info) {
			auto* isolate = info.GetIsolate();
			auto ctx = isolate->GetCurrentContext();
			try {
				for (int i = 0; i < info.Length(); ++i) {
					pending_mapflags().push_back(read_mapflag(isolate, ctx, info[i], static_cast<size_t>(i)));
				}
			} catch (const std::exception& e) {
				isolate->ThrowException(v8::Exception::Error(
				    v8::String::NewFromUtf8(isolate, e.what()).ToLocalChecked()));
			}
		}

	} // namespace

	void bind_warp_registrars(v8::Isolate* isolate, v8::Local<v8::Context> context) {
		auto global = context->Global();
		{
			auto tmpl = v8::FunctionTemplate::New(isolate, &register_warp_callback);
			auto fn = tmpl->GetFunction(context).ToLocalChecked();
			auto key = v8::String::NewFromUtf8(isolate, "registerWarp").ToLocalChecked();
			(void)global->Set(context, key, fn);
		}
		{
			auto tmpl = v8::FunctionTemplate::New(isolate, &register_mapflag_callback);
			auto fn = tmpl->GetFunction(context).ToLocalChecked();
			auto key = v8::String::NewFromUtf8(isolate, "registerMapFlag").ToLocalChecked();
			(void)global->Set(context, key, fn);
		}
	}

	int spawn_registered_warps() {
		int spawned = 0;
		int index = 0;
		for (const auto& reg : pending_warps()) {
			++index;

			int16 from_mapid = map_mapname2mapid(reg.from_map.c_str());
			if (from_mapid < 0) {
				ShowWarning("[ts-scripting] cannot spawn registerWarp #%d: source map '%s' not loaded.\n",
				    index, reg.from_map.c_str());
				continue;
			}
			// NOTE: destination resolves via the map INDEX table
			// (mapindex_name2id), not map_mapname2mapid — warps store
			// their target as an index so they still resolve after a
			// changemapserver, and npc_add_warp expects that shape.
			uint16 to_mapindex = mapindex_name2id(reg.to_map.c_str());
			if (to_mapindex == 0) {
				ShowWarning("[ts-scripting] cannot spawn registerWarp #%d: destination map '%s' not loaded.\n",
				    index, reg.to_map.c_str());
				continue;
			}

			char name[NPC_NAME_LENGTH + 1];
			safesnprintf(name, sizeof(name), "tswarp%d_%s_%d_%d", index, reg.from_map.c_str(), reg.from_x, reg.from_y);

			npc_data* nd = npc_add_warp(name, from_mapid, static_cast<int16>(reg.from_x), static_cast<int16>(reg.from_y),
			    static_cast<int16>(reg.xs), static_cast<int16>(reg.ys), to_mapindex,
			    static_cast<int16>(reg.to_x), static_cast<int16>(reg.to_y));
			if (!nd) {
				ShowWarning("[ts-scripting] npc_add_warp failed for registerWarp #%d ('%s' -> '%s').\n",
				    index, reg.from_map.c_str(), reg.to_map.c_str());
				continue;
			}
			// "warp2" triggers for hidden players too — see npc_parse_warp's
			// is_type_warp2 handling in npc.cpp.
			if (reg.trigger_on_hidden)
				nd->trigger_on_hidden = true;

			++spawned;
			ShowStatus("[ts-scripting] spawned warp '%s' %s(%d,%d) -> %s(%d,%d).\n",
			    nd->exname, reg.from_map.c_str(), reg.from_x, reg.from_y,
			    reg.to_map.c_str(), reg.to_x, reg.to_y);
		}
		return spawned;
	}

	namespace {

		// Parse the `nosave` mapflag's value ("SavePoint" or "<map>,<x>,<y>"),
		// mirroring npc_parse_mapflag's MF_NOSAVE branch in npc.cpp.
		void apply_nosave(int16 m, const MapFlagRegistration& reg) {
			union u_mapflag_args args = {};
			bool use_savepoint = true;

			if (reg.has_value && !reg.value.empty() && strcmpi(reg.value.c_str(), "SavePoint") != 0) {
				char savemap[MAP_NAME_LENGTH_EXT];
				int16 x, y;
				if (sscanf(reg.value.c_str(), "%15[^,],%6hd,%6hd", savemap, &x, &y) == 3) {
					uint16 map_idx = mapindex_name2id(savemap);
					if (map_idx) {
						args.nosave.map = map_idx;
						args.nosave.x = x;
						args.nosave.y = y;
						use_savepoint = false;
					} else {
						ShowWarning("[ts-scripting] registerMapFlag('%s','nosave'): save point map '%s' not found, using SavePoint.\n",
						    reg.map.c_str(), savemap);
					}
				} else {
					ShowWarning("[ts-scripting] registerMapFlag('%s','nosave'): value '%s' is not '<map>,<x>,<y>' or 'SavePoint', using SavePoint.\n",
					    reg.map.c_str(), reg.value.c_str());
				}
			}

			if (use_savepoint) {
				args.nosave.map = 0;
				args.nosave.x = -1;
				args.nosave.y = -1;
			}
			map_setmapflag_sub(m, MF_NOSAVE, true, &args);
		}

		// Parse a bare integer value for the handful of mapflags that carry
		// one (battleground, nocommand, restricted, jexp, bexp,
		// specialpopup, invincible_time). Returns false only when a value
		// was required but none could be obtained (missing or unparsable).
		bool parse_flag_val(const MapFlagRegistration& reg, int32& out, int32 fallback, bool required) {
			if (reg.has_value && !reg.value.empty()) {
				int32 v;
				if (sscanf(reg.value.c_str(), "%11d", &v) == 1) {
					out = v;
					return true;
				}
				ShowWarning("[ts-scripting] registerMapFlag('%s','%s'): value '%s' is not a valid integer.\n",
				    reg.map.c_str(), reg.flag.c_str(), reg.value.c_str());
				return !required;
			}
			if (required)
				return false;
			out = fallback;
			return true;
		}

	} // namespace

	int apply_registered_mapflags() {
		int applied = 0;
		for (const auto& reg : pending_mapflags()) {
			int16 m = map_mapname2mapid(reg.map.c_str());
			if (m < 0) {
				ShowWarning("[ts-scripting] cannot apply registerMapFlag('%s') on '%s': map not loaded.\n",
				    reg.flag.c_str(), reg.map.c_str());
				continue;
			}

			enum e_mapflag flag = map_getmapflag_by_name(reg.flag.c_str());
			if (flag == MF_INVALID) {
				ShowWarning("[ts-scripting] unrecognized mapflag '%s' for map '%s'.\n",
				    reg.flag.c_str(), reg.map.c_str());
				continue;
			}

			// Mirrors npc_parse_mapflag's `w4 == "off"` convention: any
			// value other than the literal "off" leaves the flag enabled.
			bool state = !(reg.has_value && strcmpi(reg.value.c_str(), "off") == 0);

			switch (flag) {
				case MF_NOSAVE:
					if (state)
						apply_nosave(m, reg);
					else
						map_setmapflag(m, MF_NOSAVE, false);
					break;

				case MF_BATTLEGROUND: {
					union u_mapflag_args args = {};
					if (state && parse_flag_val(reg, args.flag_val, 1, false))
						map_setmapflag_sub(m, MF_BATTLEGROUND, true, &args);
					else
						map_setmapflag(m, MF_BATTLEGROUND, false);
					break;
				}

				case MF_NOCOMMAND: {
					union u_mapflag_args args = {};
					if (state && parse_flag_val(reg, args.flag_val, 100, false))
						map_setmapflag_sub(m, MF_NOCOMMAND, true, &args);
					else
						map_setmapflag(m, MF_NOCOMMAND, false);
					break;
				}

				case MF_RESTRICTED: {
					union u_mapflag_args args = {};
					if (!state) {
						map_setmapflag(m, MF_RESTRICTED, false);
						break;
					}
					if (!parse_flag_val(reg, args.flag_val, 0, true)) {
						// parse_flag_val() already warned when a value was
						// present but unparsable; only add context here for
						// the "no value given at all" case.
						if (!reg.has_value)
							ShowWarning("[ts-scripting] registerMapFlag('%s','restricted'): zone value required, skipping.\n",
							    reg.map.c_str());
						continue;
					}
					map_setmapflag_sub(m, MF_RESTRICTED, true, &args);
					break;
				}

				case MF_JEXP:
				case MF_BEXP:
				case MF_SPECIALPOPUP: {
					union u_mapflag_args args = {};
					parse_flag_val(reg, args.flag_val, 0, false);
					map_setmapflag_sub(m, flag, state, &args);
					break;
				}

				case MF_INVINCIBLE_TIME: {
					union u_mapflag_args args = {};
					if (state && !parse_flag_val(reg, args.flag_val, 0, true)) {
						// parse_flag_val() already warned when a value was
						// present but unparsable; only add context here for
						// the "no value given at all" case.
						if (!reg.has_value)
							ShowWarning("[ts-scripting] registerMapFlag('%s','invincible_time'): time value required, skipping.\n",
							    reg.map.c_str());
						continue;
					}
					map_setmapflag_sub(m, MF_INVINCIBLE_TIME, state, &args);
					break;
				}

				case MF_PVP_NIGHTMAREDROP:
				case MF_SKILL_DAMAGE:
				case MF_SKILL_DURATION:
					// These need item/skill-db lookups and script constant
					// resolution to parse safely (see npc_parse_mapflag in
					// npc.cpp) — not worth guessing at here.
					ShowWarning("[ts-scripting] mapflag '%s' needs value parsing not supported by registerMapFlag; skipping (map '%s').\n",
					    reg.flag.c_str(), reg.map.c_str());
					continue;

				default:
					map_setmapflag(m, flag, state);
					break;
			}

			++applied;
			ShowStatus("[ts-scripting] applied mapflag '%s'%s on map '%s'.\n",
			    reg.flag.c_str(), state ? "" : " (off)", reg.map.c_str());
		}
		return applied;
	}

	void clear_warp_registrations() {
		pending_warps().clear();
		pending_mapflags().clear();
	}

} // namespace rathena::scripting

#endif // HAVE_TS_SCRIPTING

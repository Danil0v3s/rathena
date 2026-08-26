#ifdef HAVE_TS_SCRIPTING

#include "registrar_shop.hpp"

#include <string>
#include <vector>

#include "js_object_reader.hpp"
#include "../map.hpp"
#include "../npc.hpp"
#include "../itemdb.hpp"
#include "../../common/cbasetypes.hpp"
#include "../../common/malloc.hpp"
#include "../../common/mmo.hpp"
#include "../../common/showmsg.hpp"
#include "../../common/strlib.hpp"

namespace rathena::scripting {

	namespace {

		// Mirrors the `kind` discriminator of the TS `ShopRegistration` union
		// (npc-ts/types/registrations.d.ts) onto rAthena's npc_subtype shop
		// variants.
		enum class ShopKind {
			Shop,
			Cash,
			Item,
			Point,
			Market,
		};

		// One entry of `items`. `stock` is only meaningful for
		// NPCTYPE_MARKETSHOP.
		struct ShopItemEntry {
			t_itemid item_id{0};
			int32 price{0};
			int32 stock{-1};
		};

		// A pending registerShop() call, captured verbatim from JS. Resolved
		// into a live shop NPC by spawn_registered_shops() once maps and the
		// item database are loaded.
		struct ShopRegistration {
			ShopKind kind{ShopKind::Shop};
			std::string name;
			std::string map;
			int x{0};
			int y{0};
			int dir{0};
			int sprite{0};
			t_itemid cost_item{0};     // kind == Item
			std::string cost_variable; // kind == Point

			// Shop-wide, and only consulted for ITEMSHOP / POINTSHOP —
			// npc_shop_discount() uses it purely to decide whether the
			// Merchant Discount skill applies. There is no per-item rate
			// anywhere in the engine; the percentage comes from the
			// player's skill level.
			bool discount{false};

			std::vector<ShopItemEntry> items;
		};

		// File-local storage — kept out of the header so registrar_shop.hpp
		// stays a clean public surface. Function-local static gives us lazy,
		// well-defined initialization order.
		std::vector<ShopRegistration>& pending_shops() {
			static std::vector<ShopRegistration> instance;
			return instance;
		}

		// Pull a required array field off `obj`. Throws if missing or not
		// an array.
		v8::Local<v8::Array> require_array(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Object> obj, const char* field, const std::string& trace) {
			auto key = v8::String::NewFromUtf8(iso, field).ToLocalChecked();
			v8::Local<v8::Value> v;
			if (!obj->Get(ctx, key).ToLocal(&v) || js::is_undefined_or_null(v)) {
				throw ScriptRegistrationError(trace + ": required field '" + field + "' is missing.");
			}
			if (!v->IsArray()) {
				throw ScriptRegistrationError(
				    trace + ": field '" + field + "' must be an array; got " + js::describe_type(v) + ".");
			}
			return v.As<v8::Array>();
		}

		// Read one ShopItem / MarketShopItem entry. `is_market` selects
		// whether the per-item `stock` field is required.
		ShopItemEntry read_shop_item(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Value> raw, const std::string& trace, size_t index, bool is_market) {
			if (raw.IsEmpty() || !raw->IsObject() || raw->IsArray() || raw->IsFunction()) {
				throw ScriptRegistrationError(
				    trace + ".items[" + std::to_string(index) + "] must be an object; got " +
				    js::describe_type(raw) + ".");
			}
			auto obj = raw.As<v8::Object>();
			std::string item_trace = trace + ".items[" + std::to_string(index) + "]";

			ShopItemEntry entry;
			int32 item_id = js::require_int(iso, ctx, obj, "itemId", item_trace);
			if (item_id < 0) {
				throw ScriptRegistrationError(item_trace + ": 'itemId' must not be negative.");
			}
			entry.item_id = static_cast<t_itemid>(item_id);
			entry.price = js::require_int(iso, ctx, obj, "price", item_trace);

			if (is_market) {
				entry.stock = js::require_int(iso, ctx, obj, "stock", item_trace);
			}

			return entry;
		}

		ShopRegistration read_shop(v8::Isolate* iso, v8::Local<v8::Context> ctx,
		    v8::Local<v8::Value> raw, size_t index) {
			std::string trace = "registerShop[" + std::to_string(index) + "]";
			if (raw.IsEmpty() || !raw->IsObject() || raw->IsArray() || raw->IsFunction()) {
				throw ScriptRegistrationError(
				    trace + " must be an object; got " + js::describe_type(raw) + ".");
			}
			auto obj = raw.As<v8::Object>();

			ShopRegistration reg;
			std::string kind_str = js::require_string(iso, ctx, obj, "kind", trace);
			reg.name = js::require_string(iso, ctx, obj, "name", trace);
			trace = "registerShop('" + reg.name + "')";

			if (kind_str == "shop") {
				reg.kind = ShopKind::Shop;
			} else if (kind_str == "cash") {
				reg.kind = ShopKind::Cash;
			} else if (kind_str == "item") {
				reg.kind = ShopKind::Item;
			} else if (kind_str == "point") {
				reg.kind = ShopKind::Point;
			} else if (kind_str == "market") {
				reg.kind = ShopKind::Market;
			} else {
				throw ScriptRegistrationError(
				    trace + ": field 'kind' must be one of \"shop\", \"cash\", \"item\", \"point\", "
				    "\"market\"; got \"" + kind_str + "\".");
			}

			reg.map = js::require_string(iso, ctx, obj, "map", trace);
			reg.x = js::require_int(iso, ctx, obj, "x", trace);
			reg.y = js::require_int(iso, ctx, obj, "y", trace);
			reg.dir = js::optional_int(iso, ctx, obj, "dir", 0);
			reg.sprite = js::require_int(iso, ctx, obj, "sprite", trace);

			if (reg.kind == ShopKind::Item) {
				int32 cost_item = js::require_int(iso, ctx, obj, "costItem", trace);
				if (cost_item < 0) {
					throw ScriptRegistrationError(trace + ": 'costItem' must not be negative.");
				}
				reg.cost_item = static_cast<t_itemid>(cost_item);
			} else if (reg.kind == ShopKind::Point) {
				reg.cost_variable = js::require_string(iso, ctx, obj, "costVariable", trace);
				// Mirrors npc_parse_shop's point_str validation in npc.cpp:
				// the currency variable must be a permanent, integer-typed
				// character/account variable (no scope prefix, no '$' string
				// suffix).
				if (reg.cost_variable.empty()) {
					throw ScriptRegistrationError(trace + ": 'costVariable' must not be empty.");
				}
				char first = reg.cost_variable.front();
				if (first == '$' || first == '.' || first == '\'') {
					throw ScriptRegistrationError(
					    trace + ": 'costVariable' must be a permanent character or account based "
					    "variable (no '$', '.', or '\\'' scope prefix); got \"" + reg.cost_variable + "\".");
				}
				if (reg.cost_variable.back() == '$') {
					throw ScriptRegistrationError(
					    trace + ": 'costVariable' must be an integer variable (no '$' suffix); got \"" +
					    reg.cost_variable + "\".");
				}
			}

			// `discount` is shop-wide and only consulted for ITEMSHOP /
			// POINTSHOP — see npc_shop_discount() in npc.cpp.
			if (reg.kind == ShopKind::Item || reg.kind == ShopKind::Point) {
				auto key = v8::String::NewFromUtf8(iso, "discount").ToLocalChecked();
				v8::Local<v8::Value> v;
				if (obj->Get(ctx, key).ToLocal(&v) && !js::is_undefined_or_null(v))
					reg.discount = v->BooleanValue(iso);
			}

			bool is_market = reg.kind == ShopKind::Market;
			auto items = require_array(iso, ctx, obj, "items", trace);
			for (uint32_t i = 0; i < items->Length(); ++i) {
				v8::Local<v8::Value> item_val;
				if (!items->Get(ctx, i).ToLocal(&item_val))
					continue;
				reg.items.push_back(read_shop_item(iso, ctx, item_val, trace, i, is_market));
			}
			if (reg.items.empty()) {
				throw ScriptRegistrationError(trace + ": 'items' must be a non-empty array.");
			}

			return reg;
		}

		void register_shop_callback(const v8::FunctionCallbackInfo<v8::Value>& info) {
			auto* isolate = info.GetIsolate();
			auto ctx = isolate->GetCurrentContext();
			try {
				for (int i = 0; i < info.Length(); ++i) {
					pending_shops().push_back(read_shop(isolate, ctx, info[i], static_cast<size_t>(i)));
				}
			} catch (const std::exception& e) {
				isolate->ThrowException(v8::Exception::Error(
				    v8::String::NewFromUtf8(isolate, e.what()).ToLocalChecked()));
			}
		}

	} // namespace

	void bind_shop_registrar(v8::Isolate* isolate, v8::Local<v8::Context> context) {
		auto tmpl = v8::FunctionTemplate::New(isolate, &register_shop_callback);
		auto fn = tmpl->GetFunction(context).ToLocalChecked();
		auto key = v8::String::NewFromUtf8(isolate, "registerShop").ToLocalChecked();
		auto global = context->Global();
		(void)global->Set(context, key, fn);
	}

	int spawn_registered_shops() {
		int spawned = 0;
		for (const auto& reg : pending_shops()) {
			int16 mapid = map_mapname2mapid(reg.map.c_str());
			if (mapid < 0) {
				ShowWarning("[ts-scripting] cannot spawn shop '%s': map '%s' not loaded.\n",
				    reg.name.c_str(), reg.map.c_str());
				continue;
			}

			// Check for a name collision BEFORE allocating anything. Neither
			// the shop_item array nor npc_create_npc's npc_data is owned by
			// the engine until the install succeeds, so bailing out later
			// would leak both — and a duplicate exname is the one install
			// failure we can detect up front.
			if (npc_name2id(reg.name.c_str()) != nullptr) {
				ShowWarning("[ts-scripting] cannot spawn shop '%s': that name is already taken.\n",
				    reg.name.c_str());
				continue;
			}

			if (reg.kind == ShopKind::Item && !item_db.exists(reg.cost_item)) {
				ShowWarning("[ts-scripting] cannot spawn shop '%s': cost item id %u not found.\n",
				    reg.name.c_str(), reg.cost_item);
				continue;
			}

			// Build the validated item list first — mirrors npc_parse_shop's
			// per-item item_db check in npc.cpp. Unknown item ids are
			// skipped with a warning rather than inserted as garbage.
			struct npc_item_list* shop_item = nullptr;
			uint16 count = 0;

			for (const auto& it : reg.items) {
				std::shared_ptr<item_data> id = item_db.find(it.item_id);
				if (id == nullptr) {
					ShowWarning("[ts-scripting] shop '%s': unknown item id %u, skipping entry.\n",
					    reg.name.c_str(), it.item_id);
					continue;
				}

				int32 value = it.price;
				if (value < 0) {
					if (reg.kind == ShopKind::Shop || reg.kind == ShopKind::Market)
						value = id->value_buy;
					else
						value = 0; // cash/item/point shops don't have a zeny "buy price"
				}

				RECREATE(shop_item, struct npc_item_list, count + 1);
				shop_item[count].nameid = it.item_id;
				shop_item[count].value = value;
#if PACKETVER >= 20131223
				shop_item[count].flag = 0;
				if (reg.kind == ShopKind::Market)
					shop_item[count].qty = it.stock < -1 ? -1 : it.stock;
#endif
				count++;
			}

			if (count == 0) {
				// An empty shop can crash some clients — refuse to spawn it.
				ShowWarning("[ts-scripting] shop '%s' has no valid items, skipping.\n", reg.name.c_str());
				if (shop_item)
					aFree(shop_item);
				continue;
			}

			npc_data* nd = npc_create_npc(mapid, static_cast<int16>(reg.x), static_cast<int16>(reg.y));
			if (!nd) {
				ShowWarning("[ts-scripting] npc_create_npc returned null for shop '%s'\n", reg.name.c_str());
				aFree(shop_item);
				continue;
			}
			safestrncpy(nd->exname, reg.name.c_str(), sizeof(nd->exname));
			safestrncpy(nd->name, reg.name.c_str(), sizeof(nd->name));
			nd->class_ = reg.sprite;

			switch (reg.kind) {
				case ShopKind::Shop:
					nd->subtype = NPCTYPE_SHOP;
					break;
				case ShopKind::Cash:
					nd->subtype = NPCTYPE_CASHSHOP;
					break;
				case ShopKind::Item:
					nd->subtype = NPCTYPE_ITEMSHOP;
					break;
				case ShopKind::Point:
					nd->subtype = NPCTYPE_POINTSHOP;
					break;
				case ShopKind::Market:
					nd->subtype = NPCTYPE_MARKETSHOP;
					break;
			}

			nd->u.shop.shop_item = shop_item;
			nd->u.shop.count = count;
			// Market shops never discount (legacy hardcodes is_discount = 0).
			nd->u.shop.discount = reg.kind != ShopKind::Market && reg.discount;
			if (reg.kind == ShopKind::Item) {
				nd->u.shop.itemshop_nameid = reg.cost_item;
			} else if (reg.kind == ShopKind::Point) {
				safestrncpy(nd->u.shop.pointshop_str, reg.cost_variable.c_str(), sizeof(nd->u.shop.pointshop_str));
			}

			if (!npc_install_shop_npc(nd, mapid, static_cast<int16>(reg.dir))) {
				// Matches the established (POC-era) convention in
				// npc_spawner.cpp: on install failure (most commonly a
				// duplicate exname) we just warn and move on rather than
				// unwind partially-linked map state.
				ShowWarning("[ts-scripting] npc_install_shop_npc failed for shop '%s'\n", reg.name.c_str());
				continue;
			}

			++spawned;
			ShowStatus("[ts-scripting] spawned shop '%s' at %s (%d,%d) sprite=%d kind=%d items=%u.\n",
			    reg.name.c_str(), reg.map.c_str(), reg.x, reg.y, reg.sprite,
			    static_cast<int>(reg.kind), count);
		}
		return spawned;
	}

	void clear_shop_registrations() {
		pending_shops().clear();
	}

} // namespace rathena::scripting

#endif // HAVE_TS_SCRIPTING

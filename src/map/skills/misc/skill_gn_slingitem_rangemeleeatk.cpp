#include "skill_gn_slingitem_rangemeleeatk.hpp"
#include "../../pc.hpp"
#include "../../status.hpp"

SkillGN_SLINGITEM_RANGEMELEEATK::SkillGN_SLINGITEM_RANGEMELEEATK() : WeaponSkillImpl(GN_SLINGITEM_RANGEMELEEATK) {
}

void SkillGN_SLINGITEM_RANGEMELEEATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if( src->type == BL_PC ) {
		switch( ((map_session_data*)src)->itemid ) {
			case ITEMID_APPLE_BOMB:
				base_skillratio += 200 + status_get_str(src) + status_get_dex(src);
				break;
			case ITEMID_COCONUT_BOMB:
			case ITEMID_PINEAPPLE_BOMB:
				base_skillratio += 700 + status_get_str(src) + status_get_dex(src);
				break;
			case ITEMID_MELON_BOMB:
				base_skillratio += 400 + status_get_str(src) + status_get_dex(src);
				break;
			case ITEMID_BANANA_BOMB:
				base_skillratio += 777 + status_get_str(src) + status_get_dex(src);
				break;
			case ITEMID_BLACK_LUMP:
				base_skillratio += -100 + (status_get_str(src) + status_get_agi(src) + status_get_dex(src)) / 3;
				break;
			case ITEMID_BLACK_HARD_LUMP:
				base_skillratio += -100 + (status_get_str(src) + status_get_agi(src) + status_get_dex(src)) / 2;
				break;
			case ITEMID_VERY_HARD_LUMP:
				base_skillratio += -100 + status_get_str(src) + status_get_agi(src) + status_get_dex(src);
				break;
		}
	}
}

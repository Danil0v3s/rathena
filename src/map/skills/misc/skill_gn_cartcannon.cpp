#include "skill_gn_cartcannon.hpp"
#include "../../pc.hpp"
#include "../../status.hpp"

SkillGN_CARTCANNON::SkillGN_CARTCANNON() : WeaponSkillImpl(GN_CARTCANNON) {
}

void SkillGN_CARTCANNON::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + (250 + 20 * pc_checkskill((map_session_data*)src, GN_REMODELING_CART)) * skill_lv + 2 * status_get_int(src) / (6 - pc_checkskill((map_session_data*)src, GN_REMODELING_CART));
}

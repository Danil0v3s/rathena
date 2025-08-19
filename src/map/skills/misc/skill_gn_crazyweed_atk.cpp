#include "skill_gn_crazyweed_atk.hpp"

SkillGN_CRAZYWEED_ATK::SkillGN_CRAZYWEED_ATK() : WeaponSkillImpl(GN_CRAZYWEED_ATK) {
}

void SkillGN_CRAZYWEED_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 700 + 100 * skill_lv;
}

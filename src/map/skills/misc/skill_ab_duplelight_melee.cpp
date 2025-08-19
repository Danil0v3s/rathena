#include "skill_ab_duplelight_melee.hpp"

SkillAB_DUPLELIGHT_MELEE::SkillAB_DUPLELIGHT_MELEE() : WeaponSkillImpl(AB_DUPLELIGHT_MELEE) {
}

void SkillAB_DUPLELIGHT_MELEE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50 + 15 * skill_lv;
}

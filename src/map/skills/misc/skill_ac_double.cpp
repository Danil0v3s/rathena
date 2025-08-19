#include "skill_ac_double.hpp"

SkillAC_DOUBLE::SkillAC_DOUBLE() : WeaponSkillImpl(AC_DOUBLE) {
}

void SkillAC_DOUBLE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 10 * (skill_lv - 1);
}

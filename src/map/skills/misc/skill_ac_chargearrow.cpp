#include "skill_ac_chargearrow.hpp"

SkillAC_CHARGEARROW::SkillAC_CHARGEARROW() : WeaponSkillImpl(AC_CHARGEARROW) {
}

void SkillAC_CHARGEARROW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50;
}

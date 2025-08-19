#include "skill_su_bite.hpp"

SkillSU_BITE::SkillSU_BITE() : WeaponSkillImpl(SU_BITE) {
}

void SkillSU_BITE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100;
}

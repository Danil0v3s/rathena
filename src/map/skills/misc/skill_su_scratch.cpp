#include "skill_su_scratch.hpp"

SkillSU_SCRATCH::SkillSU_SCRATCH() : WeaponSkillImpl(SU_SCRATCH) {
}

void SkillSU_SCRATCH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -50 + 50 * skill_lv;
}

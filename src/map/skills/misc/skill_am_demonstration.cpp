#include "skill_am_demonstration.hpp"

SkillAM_DEMONSTRATION::SkillAM_DEMONSTRATION() : WeaponSkillImpl(AM_DEMONSTRATION) {
}

void SkillAM_DEMONSTRATION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 20 * skill_lv;
}

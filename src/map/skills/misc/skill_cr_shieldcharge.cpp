#include "skill_cr_shieldcharge.hpp"

SkillCR_SHIELDCHARGE::SkillCR_SHIELDCHARGE() : WeaponSkillImpl(CR_SHIELDCHARGE) {
}

void SkillCR_SHIELDCHARGE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 20 * skill_lv;
}

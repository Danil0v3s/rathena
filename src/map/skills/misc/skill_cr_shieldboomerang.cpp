#include "skill_cr_shieldboomerang.hpp"

SkillCR_SHIELDBOOMERANG::SkillCR_SHIELDBOOMERANG() : WeaponSkillImpl(CR_SHIELDBOOMERANG) {
}

void SkillCR_SHIELDBOOMERANG::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + skill_lv * 80;
#else
	base_skillratio += 30 * skill_lv;
#endif
}

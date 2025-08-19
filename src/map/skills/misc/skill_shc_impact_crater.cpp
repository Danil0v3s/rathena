#include "skill_shc_impact_crater.hpp"

SkillSHC_IMPACT_CRATER::SkillSHC_IMPACT_CRATER() : WeaponSkillImpl(SHC_IMPACT_CRATER) {
}

void SkillSHC_IMPACT_CRATER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 80 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

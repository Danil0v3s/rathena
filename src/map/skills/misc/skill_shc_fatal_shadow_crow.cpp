#include "skill_shc_fatal_shadow_crow.hpp"

SkillSHC_FATAL_SHADOW_CROW::SkillSHC_FATAL_SHADOW_CROW() : WeaponSkillImpl(SHC_FATAL_SHADOW_CROW) {
}

void SkillSHC_FATAL_SHADOW_CROW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1300 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_DRAGON)
		base_skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

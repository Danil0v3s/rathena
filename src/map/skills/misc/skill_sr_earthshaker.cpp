#include "skill_sr_earthshaker.hpp"

SkillSR_EARTHSHAKER::SkillSR_EARTHSHAKER() : WeaponSkillImpl(SR_EARTHSHAKER) {
}

void SkillSR_EARTHSHAKER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (tsc && ((tsc->option&(OPTION_HIDE|OPTION_CLOAK|OPTION_CHASEWALK)) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD) || tsc->getSCE(SC__SHADOWFORM))) {
		base_skillratio += 100 + 100 * skill_lv;
	} else {
		base_skillratio += 100 + 200 * skill_lv;
	}
	RE_LVL_DMOD(100);
}

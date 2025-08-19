#include "skill_sr_rampageblaster.hpp"

SkillSR_RAMPAGEBLASTER::SkillSR_RAMPAGEBLASTER() : WeaponSkillImpl(SR_RAMPAGEBLASTER) {
}

void SkillSR_RAMPAGEBLASTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (tsc && tsc->getSCE(SC_EARTHSHAKER)) {
		base_skillratio += 1400 + 550 * skill_lv;
		RE_LVL_DMOD(120);
	} else {
		base_skillratio += 900 + 350 * skill_lv;
		RE_LVL_DMOD(150);
	}
	if (sc->getSCE(SC_GT_CHANGE))
		base_skillratio += base_skillratio * 30 / 100;
}

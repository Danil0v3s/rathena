#include "skill_sr_gateofhell.hpp"

SkillSR_GATEOFHELL::SkillSR_GATEOFHELL() : WeaponSkillImpl(SR_GATEOFHELL) {
}

void SkillSR_GATEOFHELL::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (sc && sc->getSCE(SC_COMBO) && sc->getSCE(SC_COMBO)->val1 == SR_FALLENEMPIRE)
		base_skillratio += -100 + 800 * skill_lv;
	else
		base_skillratio += -100 + 500 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc->getSCE(SC_GT_REVITALIZE))
		base_skillratio += base_skillratio * 30 / 100;
}

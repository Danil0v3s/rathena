#include "skill_wh_galestorm.hpp"

SkillWH_GALESTORM::SkillWH_GALESTORM() : WeaponSkillImpl(WH_GALESTORM) {
}

void SkillWH_GALESTORM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1350 * skill_lv;
	base_skillratio += 10 * sstatus->con;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CALAMITYGALE) && (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH))
		base_skillratio += base_skillratio * 50 / 100;
}

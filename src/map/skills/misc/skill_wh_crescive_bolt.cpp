#include "skill_wh_crescive_bolt.hpp"

SkillWH_CRESCIVE_BOLT::SkillWH_CRESCIVE_BOLT() : WeaponSkillImpl(WH_CRESCIVE_BOLT) {
}

void SkillWH_CRESCIVE_BOLT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 500 + 1300 * skill_lv;
	base_skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	if (sc) {
		if (sc->getSCE(SC_CRESCIVEBOLT))
			base_skillratio += base_skillratio * (20 * sc->getSCE(SC_CRESCIVEBOLT)->val1) / 100;

		if (sc->getSCE(SC_CALAMITYGALE)) {
			base_skillratio += base_skillratio * 20 / 100;

			if (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH)
				base_skillratio += base_skillratio * 50 / 100;
		}
	}
}

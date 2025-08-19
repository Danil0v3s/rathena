#include "skill_bo_acidified_zone_water.hpp"

SkillBO_ACIDIFIED_ZONE_WATER::SkillBO_ACIDIFIED_ZONE_WATER() : WeaponSkillImpl(BO_ACIDIFIED_ZONE_WATER) {
}

void SkillBO_ACIDIFIED_ZONE_WATER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		base_skillratio += base_skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			base_skillratio += base_skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

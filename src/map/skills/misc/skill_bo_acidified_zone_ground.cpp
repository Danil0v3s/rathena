#include "skill_bo_acidified_zone_ground.hpp"

SkillBO_ACIDIFIED_ZONE_GROUND::SkillBO_ACIDIFIED_ZONE_GROUND() : WeaponSkillImpl(BO_ACIDIFIED_ZONE_GROUND) {
}

void SkillBO_ACIDIFIED_ZONE_GROUND::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		base_skillratio += base_skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			base_skillratio += base_skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

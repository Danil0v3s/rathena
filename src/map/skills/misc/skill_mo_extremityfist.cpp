#include "skill_mo_extremityfist.hpp"

SkillMO_EXTREMITYFIST::SkillMO_EXTREMITYFIST() : WeaponSkillImpl(MO_EXTREMITYFIST) {
}

void SkillMO_EXTREMITYFIST::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 700 + sstatus->sp * 10;
#ifdef RENEWAL
	if (wd->miscflag&1)
		base_skillratio *= 2; // More than 5 spirit balls active
#endif
	base_skillratio = min(500000,base_skillratio); //We stop at roughly 50k SP for overflow protection
}

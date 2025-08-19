#include "skill_mo_investigate.hpp"

SkillMO_INVESTIGATE::SkillMO_INVESTIGATE() : WeaponSkillImpl(MO_INVESTIGATE) {
}

void SkillMO_INVESTIGATE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + 100 * skill_lv;
	if (tsc && tsc->getSCE(SC_BLADESTOP))
		base_skillratio += base_skillratio / 2;
#else
	base_skillratio += 75 * skill_lv;
#endif
}

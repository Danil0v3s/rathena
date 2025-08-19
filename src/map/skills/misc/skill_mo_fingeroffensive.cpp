#include "skill_mo_fingeroffensive.hpp"

SkillMO_FINGEROFFENSIVE::SkillMO_FINGEROFFENSIVE() : WeaponSkillImpl(MO_FINGEROFFENSIVE) {
}

void SkillMO_FINGEROFFENSIVE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 500 + skill_lv * 200;
	if (tsc && tsc->getSCE(SC_BLADESTOP))
		base_skillratio += base_skillratio / 2;
#else
	base_skillratio += 50 * skill_lv;
#endif
}

#include "skill_kn_pierce.hpp"

SkillKN_PIERCE::SkillKN_PIERCE() : WeaponSkillImpl(KN_PIERCE) {
}

void SkillKN_PIERCE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 10 * skill_lv;
	status_change* sc = status_get_sc(src);
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		base_skillratio *= 2;
}

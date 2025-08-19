#include "skill_ske_all_in_the_sky.hpp"

SkillSKE_ALL_IN_THE_SKY::SkillSKE_ALL_IN_THE_SKY() : WeaponSkillImpl(SKE_ALL_IN_THE_SKY) {
}

void SkillSKE_ALL_IN_THE_SKY::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 + 500 * skill_lv;
	base_skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		base_skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 100 * skill_lv;
	if (sd && sd->weapontype1 == W_REVOLVER)
		base_skillratio += 50 + 300 * skill_lv;
	RE_LVL_DMOD(100);
}

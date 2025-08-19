#include "skill_nw_wild_fire.hpp"

SkillNW_WILD_FIRE::SkillNW_WILD_FIRE() : WeaponSkillImpl(NW_WILD_FIRE) {
}

void SkillNW_WILD_FIRE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1500 + 3000 * skill_lv;
	base_skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		base_skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 500 * skill_lv;
	if (sd && sd->weapontype1 == W_SHOTGUN)
		base_skillratio += 200 * skill_lv;
	RE_LVL_DMOD(100);
}

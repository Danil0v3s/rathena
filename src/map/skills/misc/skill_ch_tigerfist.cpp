#include "skill_ch_tigerfist.hpp"

SkillCH_TIGERFIST::SkillCH_TIGERFIST() : WeaponSkillImpl(CH_TIGERFIST) {
}

void SkillCH_TIGERFIST::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 400 + 150 * skill_lv;
	RE_LVL_DMOD(100);
#else
	base_skillratio += -60 + 100 * skill_lv;
#endif
	if (sc->getSCE(SC_GT_ENERGYGAIN))
		base_skillratio += base_skillratio * 50 / 100;
}

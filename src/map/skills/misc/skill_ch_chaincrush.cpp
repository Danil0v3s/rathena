#include "skill_ch_chaincrush.hpp"

SkillCH_CHAINCRUSH::SkillCH_CHAINCRUSH() : WeaponSkillImpl(CH_CHAINCRUSH) {
}

void SkillCH_CHAINCRUSH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + 200 * skill_lv;
	RE_LVL_DMOD(100);
#else
	base_skillratio += 300 + 100 * skill_lv;
#endif
	if (sc->getSCE(SC_GT_ENERGYGAIN))
		base_skillratio += base_skillratio * 50 / 100;
}

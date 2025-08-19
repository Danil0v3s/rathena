#include "skill_ra_aimedbolt.hpp"

SkillRA_AIMEDBOLT::SkillRA_AIMEDBOLT() : WeaponSkillImpl(RA_AIMEDBOLT) {
}

void SkillRA_AIMEDBOLT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (sc && sc->getSCE(SC_FEARBREEZE))
		base_skillratio += -100 + 800 + 35 * skill_lv;
	else
		base_skillratio += -100 + 500 + 20 * skill_lv;	
	RE_LVL_DMOD(100);
}

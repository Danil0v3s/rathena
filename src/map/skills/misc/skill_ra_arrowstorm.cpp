#include "skill_ra_arrowstorm.hpp"

SkillRA_ARROWSTORM::SkillRA_ARROWSTORM() : WeaponSkillImpl(RA_ARROWSTORM) {
}

void SkillRA_ARROWSTORM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (sc && sc->getSCE(SC_FEARBREEZE))
		base_skillratio += -100 + 200 + 250 * skill_lv;
	else
		base_skillratio += -100 + 200 + 180 * skill_lv;
	RE_LVL_DMOD(100);
}

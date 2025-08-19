#include "skill_dc_throwarrow.hpp"

SkillDC_THROWARROW::SkillDC_THROWARROW() : WeaponSkillImpl(DC_THROWARROW) {
}

void SkillDC_THROWARROW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 10 + 40 * skill_lv;
#else
	base_skillratio += 25 + 25 * skill_lv;
#endif
}

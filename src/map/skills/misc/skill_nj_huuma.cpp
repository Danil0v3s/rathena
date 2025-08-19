#include "skill_nj_huuma.hpp"

SkillNJ_HUUMA::SkillNJ_HUUMA() : WeaponSkillImpl(NJ_HUUMA) {
}

void SkillNJ_HUUMA::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -150 + 250 * skill_lv;
#else
	base_skillratio += 50 + 150 * skill_lv;
#endif
}

#include "skill_nj_kasumikiri.hpp"

SkillNJ_KASUMIKIRI::SkillNJ_KASUMIKIRI() : WeaponSkillImpl(NJ_KASUMIKIRI) {
}

void SkillNJ_KASUMIKIRI::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 20 * skill_lv;
#else
	base_skillratio += 10 * skill_lv;
#endif
}

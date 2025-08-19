#include "skill_ht_phantasmic.hpp"

SkillHT_PHANTASMIC::SkillHT_PHANTASMIC() : WeaponSkillImpl(HT_PHANTASMIC) {
}

void SkillHT_PHANTASMIC::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 400;
#else
	base_skillratio += 50;
#endif
}

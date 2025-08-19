#include "skill_nj_syuriken.hpp"

SkillNJ_SYURIKEN::SkillNJ_SYURIKEN() : WeaponSkillImpl(NJ_SYURIKEN) {
}

#ifdef RENEWAL
void SkillNJ_SYURIKEN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 5 * skill_lv;
}
#endif
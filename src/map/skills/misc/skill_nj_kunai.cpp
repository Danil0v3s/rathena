#include "skill_nj_kunai.hpp"

SkillNJ_KUNAI::SkillNJ_KUNAI() : WeaponSkillImpl(NJ_KUNAI) {
}

#ifdef RENEWAL
void SkillNJ_KUNAI::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 100 * skill_lv;
}
#endif

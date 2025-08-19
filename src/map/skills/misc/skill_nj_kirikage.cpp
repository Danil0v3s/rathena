#include "skill_nj_kirikage.hpp"

SkillNJ_KIRIKAGE::SkillNJ_KIRIKAGE() : WeaponSkillImpl(NJ_KIRIKAGE) {
}

void SkillNJ_KIRIKAGE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -50 + 150 * skill_lv;
#else
	base_skillratio += 100 * (skill_lv - 1);
#endif
}

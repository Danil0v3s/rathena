#include "skill_lk_headcrush.hpp"

SkillLK_HEADCRUSH::SkillLK_HEADCRUSH() : WeaponSkillImpl(LK_HEADCRUSH) {
}

void SkillLK_HEADCRUSH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 40 * skill_lv;
}

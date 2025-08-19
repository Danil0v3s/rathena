#include "skill_mh_lava_slide.hpp"

SkillMH_LAVA_SLIDE::SkillMH_LAVA_SLIDE() : WeaponSkillImpl(MH_LAVA_SLIDE) {
}

void SkillMH_LAVA_SLIDE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 50 * skill_lv;
}

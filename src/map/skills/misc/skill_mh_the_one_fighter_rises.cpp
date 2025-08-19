#include "skill_mh_the_one_fighter_rises.hpp"

SkillMH_THE_ONE_FIGHTER_RISES::SkillMH_THE_ONE_FIGHTER_RISES() : WeaponSkillImpl(MH_THE_ONE_FIGHTER_RISES) {
}

void SkillMH_THE_ONE_FIGHTER_RISES::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 580 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
}

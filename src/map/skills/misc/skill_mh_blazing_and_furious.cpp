#include "skill_mh_blazing_and_furious.hpp"

SkillMH_BLAZING_AND_FURIOUS::SkillMH_BLAZING_AND_FURIOUS() : WeaponSkillImpl(MH_BLAZING_AND_FURIOUS) {
}

void SkillMH_BLAZING_AND_FURIOUS::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 80 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
}

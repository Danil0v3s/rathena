#include "skill_mh_silvervein_rush.hpp"

SkillMH_SILVERVEIN_RUSH::SkillMH_SILVERVEIN_RUSH() : WeaponSkillImpl(MH_SILVERVEIN_RUSH) {
}

void SkillMH_SILVERVEIN_RUSH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 * skill_lv * status_get_lv(src) / 100 + sstatus->str; // !TODO: Confirm STR bonus
}

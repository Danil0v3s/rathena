#include "skill_mh_stahl_horn.hpp"

SkillMH_STAHL_HORN::SkillMH_STAHL_HORN() : WeaponSkillImpl(MH_STAHL_HORN) {
}

void SkillMH_STAHL_HORN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1000 + 300 * skill_lv * status_get_lv(src) / 150 + sstatus->vit; // !TODO: Confirm VIT bonus
}

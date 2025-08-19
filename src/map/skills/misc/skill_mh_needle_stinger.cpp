#include "skill_mh_needle_stinger.hpp"

SkillMH_NEEDLE_STINGER::SkillMH_NEEDLE_STINGER() : WeaponSkillImpl(MH_NEEDLE_STINGER) {
}

void SkillMH_NEEDLE_STINGER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 + 500 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
}

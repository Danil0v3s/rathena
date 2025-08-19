#include "skill_mh_needle_of_paralyze.hpp"

SkillMH_NEEDLE_OF_PARALYZE::SkillMH_NEEDLE_OF_PARALYZE() : WeaponSkillImpl(MH_NEEDLE_OF_PARALYZE) {
}

void SkillMH_NEEDLE_OF_PARALYZE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
}

#include "skill_bo_mystery_powder.hpp"

SkillBO_MYSTERY_POWDER::SkillBO_MYSTERY_POWDER() : WeaponSkillImpl(BO_MYSTERY_POWDER) {
}

void SkillBO_MYSTERY_POWDER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1500 + 4000 * skill_lv;
	base_skillratio += 5 * sstatus->pow;	// !TODO: check POW ratio
	RE_LVL_DMOD(100);
}

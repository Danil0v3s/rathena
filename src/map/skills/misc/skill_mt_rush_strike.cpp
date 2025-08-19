#include "skill_mt_rush_strike.hpp"

SkillMT_RUSH_STRIKE::SkillMT_RUSH_STRIKE() : WeaponSkillImpl(MT_RUSH_STRIKE) {
}

void SkillMT_RUSH_STRIKE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 3500 * skill_lv;
	base_skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	RE_LVL_DMOD(100);
}

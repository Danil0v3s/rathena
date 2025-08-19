#include "skill_lk_jointbeat.hpp"

SkillLK_JOINTBEAT::SkillLK_JOINTBEAT() : WeaponSkillImpl(LK_JOINTBEAT) {
}

void SkillLK_JOINTBEAT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 10 * skill_lv - 50;
	if (wd->miscflag & BREAK_NECK || (tsc && tsc->getSCE(SC_JOINTBEAT) && tsc->getSCE(SC_JOINTBEAT)->val2 & BREAK_NECK)) // The 2x damage is only for the BREAK_NECK ailment.
		base_skillratio *= 2;
}

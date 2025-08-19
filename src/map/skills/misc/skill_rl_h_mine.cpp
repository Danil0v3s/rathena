#include "skill_rl_h_mine.hpp"

SkillRL_H_MINE::SkillRL_H_MINE() : WeaponSkillImpl(RL_H_MINE) {
}

void SkillRL_H_MINE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (sd && sd->flicker) // Flicker explosion damage: 500 + 300 * SkillLv
		base_skillratio += -100 + 500 + 300 * skill_lv;
	else // 200 + 200 * SkillLv
		base_skillratio += -100 + 200 + 200 * skill_lv;
}

#include "skill_rl_d_tail.hpp"

SkillRL_D_TAIL::SkillRL_D_TAIL() : WeaponSkillImpl(RL_D_TAIL) {
}

void SkillRL_D_TAIL::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 500 + 200 * skill_lv;
	if (sd && (wd->miscflag & 8))
		base_skillratio *= 2;
	RE_LVL_DMOD(100);
}

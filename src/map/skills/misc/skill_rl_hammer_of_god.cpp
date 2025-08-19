#include "skill_rl_hammer_of_god.hpp"

SkillRL_HAMMER_OF_GOD::SkillRL_HAMMER_OF_GOD() : WeaponSkillImpl(RL_HAMMER_OF_GOD) {
}

void SkillRL_HAMMER_OF_GOD::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 100 * skill_lv;
	if (sd) {
		if (wd->miscflag & 8)
			base_skillratio += 400 * sd->spiritball_old;
		else
			base_skillratio += 150 * sd->spiritball_old;
	}
	RE_LVL_DMOD(100);
}

#include "skill_rl_s_storm.hpp"

SkillRL_S_STORM::SkillRL_S_STORM() : WeaponSkillImpl(RL_S_STORM) {
}

void SkillRL_S_STORM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1700 + 200 * skill_lv;
}

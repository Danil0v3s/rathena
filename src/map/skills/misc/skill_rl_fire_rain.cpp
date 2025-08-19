#include "skill_rl_fire_rain.hpp"

SkillRL_FIRE_RAIN::SkillRL_FIRE_RAIN() : WeaponSkillImpl(RL_FIRE_RAIN) {
}

void SkillRL_FIRE_RAIN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 3500 + 300 * skill_lv;
}

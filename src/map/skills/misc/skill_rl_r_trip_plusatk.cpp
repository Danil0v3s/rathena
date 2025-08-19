#include "skill_rl_r_trip_plusatk.hpp"

SkillRL_R_TRIP_PLUSATK::SkillRL_R_TRIP_PLUSATK() : WeaponSkillImpl(RL_R_TRIP_PLUSATK) {
}

void SkillRL_R_TRIP_PLUSATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 300 + 300 * skill_lv;
}

#include "skill_rl_r_trip.hpp"

SkillRL_R_TRIP::SkillRL_R_TRIP() : WeaponSkillImpl(RL_R_TRIP) {
}

void SkillRL_R_TRIP::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 350 * skill_lv;
	RE_LVL_DMOD(100);
}

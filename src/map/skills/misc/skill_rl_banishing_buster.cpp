#include "skill_rl_banishing_buster.hpp"

SkillRL_BANISHING_BUSTER::SkillRL_BANISHING_BUSTER() : WeaponSkillImpl(RL_BANISHING_BUSTER) {
}

void SkillRL_BANISHING_BUSTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1000 + 200 * skill_lv;
	RE_LVL_DMOD(100);
}

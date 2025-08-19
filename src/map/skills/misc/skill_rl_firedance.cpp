#include "skill_rl_firedance.hpp"

SkillRL_FIREDANCE::SkillRL_FIREDANCE() : WeaponSkillImpl(RL_FIREDANCE) {
}

void SkillRL_FIREDANCE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 + 100 * skill_lv;
	base_skillratio += (sd ? pc_checkskill(sd, GS_DESPERADO) * 20 : 0);
	RE_LVL_DMOD(100);
}

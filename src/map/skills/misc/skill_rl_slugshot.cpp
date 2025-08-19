#include "skill_rl_slugshot.hpp"

SkillRL_SLUGSHOT::SkillRL_SLUGSHOT() : WeaponSkillImpl(RL_SLUGSHOT) {
}

void SkillRL_SLUGSHOT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (target->type == BL_MOB)
		base_skillratio += -100 + 1200 * skill_lv;
	else
		base_skillratio += -100 + 2000 * skill_lv;
	base_skillratio *= 2 + tstatus->size;
}

#include "skill_rg_backstap.hpp"

SkillRG_BACKSTAP::SkillRG_BACKSTAP() : WeaponSkillImpl(RG_BACKSTAP) {
}

void SkillRG_BACKSTAP::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if(sd && sd->status.weapon == W_BOW && battle_config.backstab_bow_penalty)
	    base_skillratio += (200 + 40 * skill_lv) / 2;
	else
	base_skillratio += 200 + 40 * skill_lv;
}

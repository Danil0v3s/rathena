#include "skill_abc_chain_reaction_shot_atk.hpp"

SkillABC_CHAIN_REACTION_SHOT_ATK::SkillABC_CHAIN_REACTION_SHOT_ATK() : WeaponSkillImpl(ABC_CHAIN_REACTION_SHOT_ATK) {
}

void SkillABC_CHAIN_REACTION_SHOT_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 800 + 2550 * skill_lv;
	base_skillratio += 15 * sstatus->con;
	RE_LVL_DMOD(100);
}

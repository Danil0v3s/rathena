#include "skill_abc_chain_reaction_shot.hpp"

SkillABC_CHAIN_REACTION_SHOT::SkillABC_CHAIN_REACTION_SHOT() : WeaponSkillImpl(ABC_CHAIN_REACTION_SHOT) {
}

void SkillABC_CHAIN_REACTION_SHOT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 850 * skill_lv;
	base_skillratio += 15 * sstatus->con;
	RE_LVL_DMOD(100);
}

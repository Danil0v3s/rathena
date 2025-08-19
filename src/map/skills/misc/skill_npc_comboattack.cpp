#include "skill_npc_comboattack.hpp"

SkillNPC_COMBOATTACK::SkillNPC_COMBOATTACK() : WeaponSkillImpl(NPC_COMBOATTACK) {
}

void SkillNPC_COMBOATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 25 * skill_lv;
}

#include "skill_npc_darknessattack.hpp"

SkillNPC_DARKNESSATTACK::SkillNPC_DARKNESSATTACK() : WeaponSkillImpl(NPC_DARKNESSATTACK) {
}

void SkillNPC_DARKNESSATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

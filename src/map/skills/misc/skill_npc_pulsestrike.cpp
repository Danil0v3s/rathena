#include "skill_npc_pulsestrike.hpp"

SkillNPC_PULSESTRIKE::SkillNPC_PULSESTRIKE() : WeaponSkillImpl(NPC_PULSESTRIKE) {
}

void SkillNPC_PULSESTRIKE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

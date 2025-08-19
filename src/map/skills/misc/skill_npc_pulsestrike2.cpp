#include "skill_npc_pulsestrike2.hpp"

SkillNPC_PULSESTRIKE2::SkillNPC_PULSESTRIKE2() : SkillImpl(NPC_PULSESTRIKE2) {
}

void SkillNPC_PULSESTRIKE2::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

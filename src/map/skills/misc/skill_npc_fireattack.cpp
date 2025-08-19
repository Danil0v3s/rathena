#include "skill_npc_fireattack.hpp"

SkillNPC_FIREATTACK::SkillNPC_FIREATTACK() : WeaponSkillImpl(NPC_FIREATTACK) {
}

void SkillNPC_FIREATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

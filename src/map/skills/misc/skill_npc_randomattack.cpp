#include "skill_npc_randomattack.hpp"

SkillNPC_RANDOMATTACK::SkillNPC_RANDOMATTACK() : WeaponSkillImpl(NPC_RANDOMATTACK) {
}

void SkillNPC_RANDOMATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

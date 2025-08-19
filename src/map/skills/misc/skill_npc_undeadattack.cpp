#include "skill_npc_undeadattack.hpp"

SkillNPC_UNDEADATTACK::SkillNPC_UNDEADATTACK() : WeaponSkillImpl(NPC_UNDEADATTACK) {
}

void SkillNPC_UNDEADATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

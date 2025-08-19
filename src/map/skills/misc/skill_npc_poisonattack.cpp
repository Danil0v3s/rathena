#include "skill_npc_poisonattack.hpp"

SkillNPC_POISONATTACK::SkillNPC_POISONATTACK() : WeaponSkillImpl(NPC_POISONATTACK) {
}

void SkillNPC_POISONATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

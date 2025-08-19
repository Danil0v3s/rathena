#include "skill_npc_waterattack.hpp"

SkillNPC_WATERATTACK::SkillNPC_WATERATTACK() : WeaponSkillImpl(NPC_WATERATTACK) {
}

void SkillNPC_WATERATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

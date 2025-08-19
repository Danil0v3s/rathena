#include "skill_npc_blooddrain.hpp"

SkillNPC_BLOODDRAIN::SkillNPC_BLOODDRAIN() : WeaponSkillImpl(NPC_BLOODDRAIN) {
}

void SkillNPC_BLOODDRAIN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

#include "skill_npc_holyattack.hpp"

SkillNPC_HOLYATTACK::SkillNPC_HOLYATTACK() : WeaponSkillImpl(NPC_HOLYATTACK) {
}

void SkillNPC_HOLYATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

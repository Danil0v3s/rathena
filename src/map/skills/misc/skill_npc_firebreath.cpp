#include "skill_npc_firebreath.hpp"

SkillNPC_FIREBREATH::SkillNPC_FIREBREATH() : WeaponSkillImpl(NPC_FIREBREATH) {
}

void SkillNPC_FIREBREATH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

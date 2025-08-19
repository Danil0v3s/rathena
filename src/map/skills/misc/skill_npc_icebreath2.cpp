#include "skill_npc_icebreath2.hpp"

SkillNPC_ICEBREATH2::SkillNPC_ICEBREATH2() : WeaponSkillImpl(NPC_ICEBREATH2) {
}

void SkillNPC_ICEBREATH2::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

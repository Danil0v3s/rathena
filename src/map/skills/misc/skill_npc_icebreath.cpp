#include "skill_npc_icebreath.hpp"

SkillNPC_ICEBREATH::SkillNPC_ICEBREATH() : WeaponSkillImpl(NPC_ICEBREATH) {
}

void SkillNPC_ICEBREATH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

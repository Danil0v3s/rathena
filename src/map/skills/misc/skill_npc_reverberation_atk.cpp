#include "skill_npc_reverberation_atk.hpp"

SkillNPC_REVERBERATION_ATK::SkillNPC_REVERBERATION_ATK() : WeaponSkillImpl(NPC_REVERBERATION_ATK) {
}

void SkillNPC_REVERBERATION_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 400 + 200 * skill_lv;
}

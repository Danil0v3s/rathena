#include "skill_npc_telekinesisattack.hpp"

SkillNPC_TELEKINESISATTACK::SkillNPC_TELEKINESISATTACK() : WeaponSkillImpl(NPC_TELEKINESISATTACK) {
}

void SkillNPC_TELEKINESISATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

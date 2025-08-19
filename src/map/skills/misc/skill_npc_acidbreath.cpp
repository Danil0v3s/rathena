#include "skill_npc_acidbreath.hpp"

SkillNPC_ACIDBREATH::SkillNPC_ACIDBREATH() : WeaponSkillImpl(NPC_ACIDBREATH) {
}

void SkillNPC_ACIDBREATH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

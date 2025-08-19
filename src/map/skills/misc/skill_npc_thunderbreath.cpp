#include "skill_npc_thunderbreath.hpp"

SkillNPC_THUNDERBREATH::SkillNPC_THUNDERBREATH() : WeaponSkillImpl(NPC_THUNDERBREATH) {
}

void SkillNPC_THUNDERBREATH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

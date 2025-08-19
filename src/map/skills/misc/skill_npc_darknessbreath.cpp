#include "skill_npc_darknessbreath.hpp"

SkillNPC_DARKNESSBREATH::SkillNPC_DARKNESSBREATH() : WeaponSkillImpl(NPC_DARKNESSBREATH) {
}

void SkillNPC_DARKNESSBREATH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

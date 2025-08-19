#include "skill_npc_dragonbreath.hpp"

SkillNPC_DRAGONBREATH::SkillNPC_DRAGONBREATH() : WeaponSkillImpl(NPC_DRAGONBREATH) {
}

void SkillNPC_DRAGONBREATH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (skill_lv > 5)
		base_skillratio += 500 + 500 * (skill_lv - 5);	// Level 6-10 is using water element, like RK_DRAGONBREATH_WATER
	else
		base_skillratio += 500 + 500 * skill_lv;	// Level 1-5 is using fire element, like RK_DRAGONBREATH
}

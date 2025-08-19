#include "skill_npc_arrowstorm.hpp"

SkillNPC_ARROWSTORM::SkillNPC_ARROWSTORM() : WeaponSkillImpl(NPC_ARROWSTORM) {
}

void SkillNPC_ARROWSTORM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (skill_lv > 4)
		base_skillratio += 1900;
	else
		base_skillratio += 900;
}

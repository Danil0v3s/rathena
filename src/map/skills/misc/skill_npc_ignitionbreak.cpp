#include "skill_npc_ignitionbreak.hpp"

SkillNPC_IGNITIONBREAK::SkillNPC_IGNITIONBREAK() : WeaponSkillImpl(NPC_IGNITIONBREAK) {
}

void SkillNPC_IGNITIONBREAK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	// 3x3 cell Damage   = 1000  1500  2000  2500  3000 %
	// 7x7 cell Damage   = 750   1250  1750  2250  2750 %
	// 11x11 cell Damage = 500   1000  1500  2000  2500 %
	int32 i = distance_bl(src,target);
	if (i < 2)
		base_skillratio += -100 + 500 * (skill_lv + 1);
	else if (i < 4)
		base_skillratio += -100 + 250 + 500 * skill_lv;
	else
		base_skillratio += -100 + 500 * skill_lv;
}

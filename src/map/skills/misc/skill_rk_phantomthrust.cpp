#include "skill_rk_phantomthrust.hpp"

SkillRK_PHANTOMTHRUST::SkillRK_PHANTOMTHRUST() : WeaponSkillImpl(RK_PHANTOMTHRUST) {
}

void SkillRK_PHANTOMTHRUST::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 50 * skill_lv + 10 * (sd ? pc_checkskill(sd,KN_SPEARMASTERY) : 5);
	RE_LVL_DMOD(150); // Base level bonus.
}

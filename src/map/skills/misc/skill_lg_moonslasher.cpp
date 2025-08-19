#include "skill_lg_moonslasher.hpp"

SkillLG_MOONSLASHER::SkillLG_MOONSLASHER() : WeaponSkillImpl(LG_MOONSLASHER) {
}

void SkillLG_MOONSLASHER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 120 * skill_lv + ((sd) ? pc_checkskill(sd,LG_OVERBRAND) * 80 : 0);
	RE_LVL_DMOD(100);
}

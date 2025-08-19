#include "skill_lg_rageburst.hpp"

SkillLG_RAGEBURST::SkillLG_RAGEBURST() : WeaponSkillImpl(LG_RAGEBURST) {
}

void SkillLG_RAGEBURST::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (sd && sd->spiritball_old)
		base_skillratio += -100 + 200 * sd->spiritball_old + (status_get_max_hp(src) - status_get_hp(src)) / 100;
	else
		base_skillratio += 2900 + (status_get_max_hp(src) - status_get_hp(src));
	RE_LVL_DMOD(100);
}

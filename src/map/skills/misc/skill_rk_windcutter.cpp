#include "skill_rk_windcutter.hpp"

SkillRK_WINDCUTTER::SkillRK_WINDCUTTER() : WeaponSkillImpl(RK_WINDCUTTER) {
}

void SkillRK_WINDCUTTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (sd) {
		if (sd->weapontype1 == W_2HSWORD)
			base_skillratio += -100 + 250 * skill_lv;
		else if (sd->weapontype1 == W_1HSPEAR || sd->weapontype1 == W_2HSPEAR)
			base_skillratio += -100 + 400 * skill_lv;
		else
			base_skillratio += -100 + 300 * skill_lv;
	} else
		base_skillratio += -100 + 300 * skill_lv;
	RE_LVL_DMOD(100);
}

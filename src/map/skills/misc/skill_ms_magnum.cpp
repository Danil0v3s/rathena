#include "skill_ms_magnum.hpp"

SkillMS_MAGNUM::SkillMS_MAGNUM() : WeaponSkillImpl(MS_MAGNUM) {
}

void SkillMS_MAGNUM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if(wd->miscflag == 1)
		base_skillratio += 20 * skill_lv; //Inner 3x3 circle takes 100%+20%*level damage [Playtester]
	else
		base_skillratio += 10 * skill_lv; //Outer 5x5 circle takes 100%+10%*level damage [Playtester]
}

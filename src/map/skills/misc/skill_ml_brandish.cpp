#include "skill_ml_brandish.hpp"

SkillML_BRANDISH::SkillML_BRANDISH() : WeaponSkillImpl(ML_BRANDISH) {
}

void SkillML_BRANDISH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	int32 ratio = 100 + 20 * skill_lv;

	base_skillratio += -100 + ratio;
	if(skill_lv > 3 && wd->miscflag == 0)
		base_skillratio += ratio / 2;
	if(skill_lv > 6 && wd->miscflag == 0)
		base_skillratio += ratio / 4;
	if(skill_lv > 9 && wd->miscflag == 0)
		base_skillratio += ratio / 8;
	if(skill_lv > 6 && wd->miscflag == 1)
		base_skillratio += ratio / 2;
	if(skill_lv > 9 && wd->miscflag == 1)
		base_skillratio += ratio / 4;
	if(skill_lv > 9 && wd->miscflag == 2)
		base_skillratio += ratio / 2;
}

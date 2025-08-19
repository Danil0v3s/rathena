#include "skill_tk_jumpkick.hpp"

SkillTK_JUMPKICK::SkillTK_JUMPKICK() : WeaponSkillImpl(TK_JUMPKICK) {
}

void SkillTK_JUMPKICK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	//Different damage formulas depending on damage trigger
	if (sc && sc->getSCE(SC_COMBO) && sc->getSCE(SC_COMBO)->val1 == skill_id)
		base_skillratio += -100 + 4 * status_get_lv(src); //Tumble formula [4%*baselevel]
	else if (wd->miscflag) {
		base_skillratio += -100 + 4 * status_get_lv(src); //Running formula [4%*baselevel]
		if (sc && sc->getSCE(SC_SPURT)) //Spurt formula [8%*baselevel]
			base_skillratio *= 2;
	}
	else
		base_skillratio += -70 + 10 * skill_lv;
}

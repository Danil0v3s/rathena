#include "skill_sr_knucklearrow.hpp"

SkillSR_KNUCKLEARROW::SkillSR_KNUCKLEARROW() : WeaponSkillImpl(SR_KNUCKLEARROW) {
}

void SkillSR_KNUCKLEARROW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (wd->miscflag&4) { // ATK [(Skill Level x 150) + (1000 x Target current weight / Maximum weight) + (Target Base Level x 5) x (Caster Base Level / 150)] %
		base_skillratio += -100 + 150 * skill_lv + status_get_lv(target) * 5;
		if (tsd && tsd->weight)
			base_skillratio += pc_getpercentweight(*tsd);
		RE_LVL_DMOD(150);
	} else {
		if (status_get_class_(target) == CLASS_BOSS)
			base_skillratio += 400 + 200 * skill_lv;
		else // ATK [(Skill Level x 100 + 500) x Caster Base Level / 100] %
			base_skillratio += 400 + 100 * skill_lv;
		RE_LVL_DMOD(100);
	}
	if (sc->getSCE(SC_GT_CHANGE))
		base_skillratio += base_skillratio * 30 / 100;
}

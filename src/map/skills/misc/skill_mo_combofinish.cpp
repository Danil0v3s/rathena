#include "skill_mo_combofinish.hpp"

SkillMO_COMBOFINISH::SkillMO_COMBOFINISH() : WeaponSkillImpl(MO_COMBOFINISH) {
}

void SkillMO_COMBOFINISH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 450 + 50 * skill_lv + sstatus->str; // !TODO: How does STR play a role?
#else
	base_skillratio += 140 + 60 * skill_lv;
#endif
	if (sc->getSCE(SC_GT_ENERGYGAIN))
		base_skillratio += base_skillratio * 50 / 100;
}

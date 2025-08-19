#include "skill_bo_explosive_powder.hpp"

SkillBO_EXPLOSIVE_POWDER::SkillBO_EXPLOSIVE_POWDER() : WeaponSkillImpl(BO_EXPLOSIVE_POWDER) {
}

void SkillBO_EXPLOSIVE_POWDER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 500 + 650 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	if (sc && sc->getSCE(SC_RESEARCHREPORT))
		base_skillratio += 100 * skill_lv;
	RE_LVL_DMOD(100);
}

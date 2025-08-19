#include "skill_bo_mayhemic_thorns.hpp"

SkillBO_MAYHEMIC_THORNS::SkillBO_MAYHEMIC_THORNS() : WeaponSkillImpl(BO_MAYHEMIC_THORNS) {
}

void SkillBO_MAYHEMIC_THORNS::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 + 300 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	if (sc && sc->getSCE(SC_RESEARCHREPORT))
		base_skillratio += 150;
	RE_LVL_DMOD(100);
}

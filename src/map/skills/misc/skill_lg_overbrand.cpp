#include "skill_lg_overbrand.hpp"

SkillLG_OVERBRAND::SkillLG_OVERBRAND() : WeaponSkillImpl(LG_OVERBRAND) {
}

void SkillLG_OVERBRAND::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if(sc && sc->getSCE(SC_OVERBRANDREADY))
		base_skillratio += -100 + 500 * skill_lv;
	else
		base_skillratio += -100 + 350 * skill_lv;
	base_skillratio += ((sd) ? pc_checkskill(sd, CR_SPEARQUICKEN) * 50 : 0);
	RE_LVL_DMOD(100);
}

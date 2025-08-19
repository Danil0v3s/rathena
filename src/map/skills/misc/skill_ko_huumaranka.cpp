#include "skill_ko_huumaranka.hpp"

SkillKO_HUUMARANKA::SkillKO_HUUMARANKA() : WeaponSkillImpl(KO_HUUMARANKA) {
}

void SkillKO_HUUMARANKA::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 150 * skill_lv + sstatus->str + (sd ? pc_checkskill(sd,NJ_HUUMA) * 100 : 0);
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		base_skillratio += base_skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
}

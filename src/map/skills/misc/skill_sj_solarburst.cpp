#include "skill_sj_solarburst.hpp"

SkillSJ_SOLARBURST::SkillSJ_SOLARBURST() : WeaponSkillImpl(SJ_SOLARBURST) {
}

void SkillSJ_SOLARBURST::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 900 + 220 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFSUN))
		base_skillratio += base_skillratio * sc->getSCE(SC_LIGHTOFSUN)->val2 / 100;
}

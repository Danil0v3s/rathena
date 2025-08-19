#include "skill_sj_fullmoonkick.hpp"

SkillSJ_FULLMOONKICK::SkillSJ_FULLMOONKICK() : WeaponSkillImpl(SJ_FULLMOONKICK) {
}

void SkillSJ_FULLMOONKICK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 1000 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFMOON))
		base_skillratio += base_skillratio * sc->getSCE(SC_LIGHTOFMOON)->val2 / 100;
}

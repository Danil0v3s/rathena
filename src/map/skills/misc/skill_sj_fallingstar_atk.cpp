#include "skill_sj_fallingstar_atk.hpp"

SkillSJ_FALLINGSTAR_ATK::SkillSJ_FALLINGSTAR_ATK() : WeaponSkillImpl(SJ_FALLINGSTAR_ATK) {
}

void SkillSJ_FALLINGSTAR_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFSTAR))
		base_skillratio += base_skillratio * sc->getSCE(SC_LIGHTOFSTAR)->val2 / 100;
}

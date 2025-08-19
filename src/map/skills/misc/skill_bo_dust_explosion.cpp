#include "skill_bo_dust_explosion.hpp"

SkillBO_DUST_EXPLOSION::SkillBO_DUST_EXPLOSION() : WeaponSkillImpl(BO_DUST_EXPLOSION) {
}

void SkillBO_DUST_EXPLOSION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 450 + 600 * skill_lv;
	base_skillratio += 5 * sstatus->pow;	// !TODO: check POW ratio
	if (sc && sc->getSCE(SC_RESEARCHREPORT))
		base_skillratio += 200 * skill_lv;
	RE_LVL_DMOD(100);
}

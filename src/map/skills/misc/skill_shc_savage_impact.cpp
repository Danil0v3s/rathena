#include "skill_shc_savage_impact.hpp"

SkillSHC_SAVAGE_IMPACT::SkillSHC_SAVAGE_IMPACT() : WeaponSkillImpl(SHC_SAVAGE_IMPACT) {
}

void SkillSHC_SAVAGE_IMPACT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 105 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_SHADOW_EXCEED ) ){
		base_skillratio += 20 * skill_lv + 3 * sstatus->pow;	// !TODO: check POW ratio
	}

	RE_LVL_DMOD(100);
}

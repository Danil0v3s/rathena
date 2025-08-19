#include "skill_shc_cross_slash.hpp"

SkillSHC_CROSS_SLASH::SkillSHC_CROSS_SLASH() : WeaponSkillImpl(SHC_CROSS_SLASH) {
}

void SkillSHC_CROSS_SLASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 300 * skill_lv;
	base_skillratio += 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_SHADOW_EXCEED ) ) {
		base_skillratio += 60 * skill_lv;
		base_skillratio += 2 * sstatus->pow;
	}
	RE_LVL_DMOD(100);
}

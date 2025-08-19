#include "skill_shc_eternal_slash.hpp"

SkillSHC_ETERNAL_SLASH::SkillSHC_ETERNAL_SLASH() : WeaponSkillImpl(SHC_ETERNAL_SLASH) {
}

void SkillSHC_ETERNAL_SLASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 300 * skill_lv + 2 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_SHADOW_EXCEED ) ){
		base_skillratio += 120 * skill_lv + sstatus->pow;
	}

	RE_LVL_DMOD(100);
}

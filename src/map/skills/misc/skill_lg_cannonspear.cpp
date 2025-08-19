#include "skill_lg_cannonspear.hpp"

SkillLG_CANNONSPEAR::SkillLG_CANNONSPEAR() : WeaponSkillImpl(LG_CANNONSPEAR) {
}

void SkillLG_CANNONSPEAR::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + skill_lv * ( 120 + sstatus->str );

	if( sc != nullptr && sc->getSCE( SC_SPEAR_SCAR ) ){
		base_skillratio += 400;
	}

	RE_LVL_DMOD(100);
}

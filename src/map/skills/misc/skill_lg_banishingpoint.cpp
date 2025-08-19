#include "skill_lg_banishingpoint.hpp"

SkillLG_BANISHINGPOINT::SkillLG_BANISHINGPOINT() : WeaponSkillImpl(LG_BANISHINGPOINT) {
}

void SkillLG_BANISHINGPOINT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + ( 100 * skill_lv );

	if( sd != nullptr ){
		base_skillratio += pc_checkskill( sd, SM_BASH ) * 70;
	}

	if( sc != nullptr && sc->getSCE( SC_SPEAR_SCAR ) ){
		base_skillratio += 800;
	}

	RE_LVL_DMOD(100);
}

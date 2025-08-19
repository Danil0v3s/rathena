#include "skill_lg_earthdrive.hpp"

SkillLG_EARTHDRIVE::SkillLG_EARTHDRIVE() : WeaponSkillImpl(LG_EARTHDRIVE) {
}

void SkillLG_EARTHDRIVE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 380 * skill_lv + sstatus->str + sstatus->vit; // !TODO: What's the STR/VIT bonus?

	if( sc != nullptr && sc->getSCE( SC_SHIELD_POWER ) ){
		base_skillratio += skill_lv * 37 * pc_checkskill( sd, IG_SHIELD_MASTERY );
	}

	RE_LVL_DMOD(100);
}

#include "skill_tr_roseblossom_atk.hpp"

SkillTR_ROSEBLOSSOM_ATK::SkillTR_ROSEBLOSSOM_ATK() : WeaponSkillImpl(TR_ROSEBLOSSOM_ATK) {
}

void SkillTR_ROSEBLOSSOM_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 + 2800 * skill_lv;

	if (sd && pc_checkskill(sd, TR_STAGE_MANNER) > 0)
		base_skillratio += 3 * sstatus->con;

	if( tsc != nullptr && tsc->getSCE( SC_SOUNDBLEND ) ){
		base_skillratio += 200 * skill_lv;
	}

	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY)) {
		base_skillratio *= 2;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			base_skillratio += base_skillratio * 50 / 100;
	}
}

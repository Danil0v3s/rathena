#include "skill_tr_roseblossom.hpp"

SkillTR_ROSEBLOSSOM::SkillTR_ROSEBLOSSOM() : WeaponSkillImpl(TR_ROSEBLOSSOM) {
}

void SkillTR_ROSEBLOSSOM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 + 2000 * skill_lv;

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

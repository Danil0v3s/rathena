#include "skill_tr_rhythmshooting.hpp"

SkillTR_RHYTHMSHOOTING::SkillTR_RHYTHMSHOOTING() : WeaponSkillImpl(TR_RHYTHMSHOOTING) {
}

void SkillTR_RHYTHMSHOOTING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 550 + 950 * skill_lv;

	if (sd && pc_checkskill(sd, TR_STAGE_MANNER) > 0)
		base_skillratio += 5 * sstatus->con;

	if (tsc && tsc->getSCE(SC_SOUNDBLEND)) {
		base_skillratio += 300 + 100 * skill_lv;
		base_skillratio += 2 * sstatus->con;
	}

	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY)) {
		base_skillratio *= 2;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			base_skillratio += base_skillratio * 50 / 100;
	}
}

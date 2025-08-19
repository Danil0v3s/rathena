#include "skill_cd_effligo.hpp"

SkillCD_EFFLIGO::SkillCD_EFFLIGO() : WeaponSkillImpl(CD_EFFLIGO) {
}

void SkillCD_EFFLIGO::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1650 * skill_lv + 7 * sstatus->pow;
	base_skillratio += 8 * pc_checkskill( sd, CD_MACE_BOOK_M );
	if (tstatus->race == RC_UNDEAD || tstatus->race == RC_DEMON) {
		base_skillratio += 150 * skill_lv;
		base_skillratio += 7 * pc_checkskill( sd, CD_MACE_BOOK_M );
	}
	RE_LVL_DMOD(100);
}

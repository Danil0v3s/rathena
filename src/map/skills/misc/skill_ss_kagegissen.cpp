#include "skill_ss_kagegissen.hpp"

SkillSS_KAGEGISSEN::SkillSS_KAGEGISSEN() : WeaponSkillImpl(SS_KAGEGISSEN) {
}

void SkillSS_KAGEGISSEN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1600 + 700 * skill_lv;
	base_skillratio += pc_checkskill( sd, SS_KAGENOMAI ) * 100 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
		base_skillratio = base_skillratio * 3 / 10;
}

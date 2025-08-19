#include "skill_ss_kunaiwaikyoku.hpp"

SkillSS_KUNAIWAIKYOKU::SkillSS_KUNAIWAIKYOKU() : WeaponSkillImpl(SS_KUNAIWAIKYOKU) {
}

void SkillSS_KUNAIWAIKYOKU::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 300 + 600 * skill_lv;
	base_skillratio += pc_checkskill( sd, SS_KUNAIKUSSETSU ) * 10 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
		base_skillratio = base_skillratio * 3 / 10;
}

#include "skill_ss_kunaikussetsu.hpp"

SkillSS_KUNAIKUSSETSU::SkillSS_KUNAIKUSSETSU() : WeaponSkillImpl(SS_KUNAIKUSSETSU) {
}

void SkillSS_KUNAIKUSSETSU::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 + 360 * skill_lv;
	base_skillratio += pc_checkskill( sd, SS_KUNAIKAITEN ) * 10 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

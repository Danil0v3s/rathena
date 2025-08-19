#include "skill_ss_fuumakouchiku.hpp"

SkillSS_FUUMAKOUCHIKU::SkillSS_FUUMAKOUCHIKU() : WeaponSkillImpl(SS_FUUMAKOUCHIKU) {
}

void SkillSS_FUUMAKOUCHIKU::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 600 + 400 * skill_lv;
	if( wd->miscflag&SKILL_ALTDMG_FLAG ){
		base_skillratio += 200;
	}
	base_skillratio += pc_checkskill( sd, SS_FUUMASHOUAKU ) * 30 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

#include "skill_ss_fuumashouaku.hpp"

SkillSS_FUUMASHOUAKU::SkillSS_FUUMASHOUAKU() : WeaponSkillImpl(SS_FUUMASHOUAKU) {
}

void SkillSS_FUUMASHOUAKU::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 700 + 200 * skill_lv;
	base_skillratio += pc_checkskill( sd, SS_FUUMAKOUCHIKU ) * 5 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

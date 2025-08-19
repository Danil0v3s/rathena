#include "skill_ss_kunaikaiten.hpp"

SkillSS_KUNAIKAITEN::SkillSS_KUNAIKAITEN() : WeaponSkillImpl(SS_KUNAIKAITEN) {
}

void SkillSS_KUNAIKAITEN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 800 + 700 * skill_lv;
	base_skillratio += pc_checkskill( sd, SS_KUNAIWAIKYOKU ) * 70 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

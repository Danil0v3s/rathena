#include "skill_ss_shimiru.hpp"

SkillSS_SHIMIRU::SkillSS_SHIMIRU() : WeaponSkillImpl(SS_SHIMIRU) {
}

void SkillSS_SHIMIRU::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 700 * skill_lv;
	base_skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

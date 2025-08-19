#include "skill_iq_third_punish.hpp"

SkillIQ_THIRD_PUNISH::SkillIQ_THIRD_PUNISH() : WeaponSkillImpl(IQ_THIRD_PUNISH) {
}

void SkillIQ_THIRD_PUNISH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 450 + 1800 * skill_lv;
	base_skillratio += 10 * sstatus->pow;
	RE_LVL_DMOD(100);
}

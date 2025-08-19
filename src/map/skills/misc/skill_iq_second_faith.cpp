#include "skill_iq_second_faith.hpp"

SkillIQ_SECOND_FAITH::SkillIQ_SECOND_FAITH() : WeaponSkillImpl(IQ_SECOND_FAITH) {
}

void SkillIQ_SECOND_FAITH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 100 + 2300 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

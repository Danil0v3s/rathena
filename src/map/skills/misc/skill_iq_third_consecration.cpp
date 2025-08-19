#include "skill_iq_third_consecration.hpp"

SkillIQ_THIRD_CONSECRATION::SkillIQ_THIRD_CONSECRATION() : WeaponSkillImpl(IQ_THIRD_CONSECRATION) {
}

void SkillIQ_THIRD_CONSECRATION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 700 * skill_lv + 10 * sstatus->pow;
	RE_LVL_DMOD(100);
}

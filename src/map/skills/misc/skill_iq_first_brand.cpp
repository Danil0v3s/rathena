#include "skill_iq_first_brand.hpp"

SkillIQ_FIRST_BRAND::SkillIQ_FIRST_BRAND() : WeaponSkillImpl(IQ_FIRST_BRAND) {
}

void SkillIQ_FIRST_BRAND::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1200 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

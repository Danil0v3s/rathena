#include "skill_iq_oleum_sanctum.hpp"

SkillIQ_OLEUM_SANCTUM::SkillIQ_OLEUM_SANCTUM() : WeaponSkillImpl(IQ_OLEUM_SANCTUM) {
}

void SkillIQ_OLEUM_SANCTUM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 500 + 2000 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

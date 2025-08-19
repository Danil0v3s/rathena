#include "skill_ht_power.hpp"

SkillHT_POWER::SkillHT_POWER() : WeaponSkillImpl(HT_POWER) {
}

void SkillHT_POWER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	status_data* sstatus = status_get_status_data(src);
	base_skillratio += -50 + 8 * sstatus->str;
}

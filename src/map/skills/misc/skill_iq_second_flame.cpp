#include "skill_iq_second_flame.hpp"

SkillIQ_SECOND_FLAME::SkillIQ_SECOND_FLAME() : WeaponSkillImpl(IQ_SECOND_FLAME) {
}

void SkillIQ_SECOND_FLAME::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 + 2900 * skill_lv + 9 * sstatus->pow;
	RE_LVL_DMOD(100);
}

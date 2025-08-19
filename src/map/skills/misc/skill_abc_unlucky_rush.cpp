#include "skill_abc_unlucky_rush.hpp"

SkillABC_UNLUCKY_RUSH::SkillABC_UNLUCKY_RUSH() : WeaponSkillImpl(ABC_UNLUCKY_RUSH) {
}

void SkillABC_UNLUCKY_RUSH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 100 + 300 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

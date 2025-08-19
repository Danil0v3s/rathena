#include "skill_mt_a_machine.hpp"

SkillMT_A_MACHINE::SkillMT_A_MACHINE() : WeaponSkillImpl(MT_A_MACHINE) {
}

void SkillMT_A_MACHINE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

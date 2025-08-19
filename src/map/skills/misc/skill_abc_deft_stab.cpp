#include "skill_abc_deft_stab.hpp"

SkillABC_DEFT_STAB::SkillABC_DEFT_STAB() : WeaponSkillImpl(ABC_DEFT_STAB) {
}

void SkillABC_DEFT_STAB::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 700 + 550 * skill_lv;
	base_skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
}

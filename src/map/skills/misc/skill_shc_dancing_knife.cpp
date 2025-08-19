#include "skill_shc_dancing_knife.hpp"

SkillSHC_DANCING_KNIFE::SkillSHC_DANCING_KNIFE() : WeaponSkillImpl(SHC_DANCING_KNIFE) {
}

void SkillSHC_DANCING_KNIFE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

#include "skill_nc_armscannon.hpp"

SkillNC_ARMSCANNON::SkillNC_ARMSCANNON() : WeaponSkillImpl(NC_ARMSCANNON) {
}

void SkillNC_ARMSCANNON::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 400 + 350 * skill_lv;
	RE_LVL_DMOD(100);
}

#include "skill_nc_coldslower.hpp"

SkillNC_COLDSLOWER::SkillNC_COLDSLOWER() : WeaponSkillImpl(NC_COLDSLOWER) {
}

void SkillNC_COLDSLOWER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 200 + 300 * skill_lv;
	RE_LVL_DMOD(150);
}

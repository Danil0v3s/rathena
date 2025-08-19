#include "skill_mo_chaincombo.hpp"

SkillMO_CHAINCOMBO::SkillMO_CHAINCOMBO() : WeaponSkillImpl(MO_CHAINCOMBO) {
}

void SkillMO_CHAINCOMBO::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 150 + 50 * skill_lv;
	if (sd && sd->status.weapon == W_KNUCKLE)
		base_skillratio *= 2;
#else
	base_skillratio += 50 + 50 * skill_lv;
#endif
}

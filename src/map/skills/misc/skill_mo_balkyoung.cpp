#include "skill_mo_balkyoung.hpp"

SkillMO_BALKYOUNG::SkillMO_BALKYOUNG() : WeaponSkillImpl(MO_BALKYOUNG) {
}

void SkillMO_BALKYOUNG::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 700;
#else
	base_skillratio += 200;
#endif
}

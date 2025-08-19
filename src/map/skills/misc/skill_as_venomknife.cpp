#include "skill_as_venomknife.hpp"

SkillAS_VENOMKNIFE::SkillAS_VENOMKNIFE() : WeaponSkillImpl(AS_VENOMKNIFE) {
}

void SkillAS_VENOMKNIFE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 400;
}

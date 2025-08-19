#include "skill_el_typoon_mis.hpp"

SkillEL_TYPOON_MIS::SkillEL_TYPOON_MIS() : WeaponSkillImpl(EL_TYPOON_MIS) {
}

void SkillEL_TYPOON_MIS::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 900;
}

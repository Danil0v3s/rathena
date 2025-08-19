#include "skill_el_water_screw_atk.hpp"

SkillEL_WATER_SCREW_ATK::SkillEL_WATER_SCREW_ATK() : WeaponSkillImpl(EL_WATER_SCREW_ATK) {
}

void SkillEL_WATER_SCREW_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 900;
}

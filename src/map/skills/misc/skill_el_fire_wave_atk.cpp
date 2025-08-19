#include "skill_el_fire_wave_atk.hpp"

SkillEL_FIRE_WAVE_ATK::SkillEL_FIRE_WAVE_ATK() : WeaponSkillImpl(EL_FIRE_WAVE_ATK) {
}

void SkillEL_FIRE_WAVE_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 500;
}

#include "skill_el_wind_slash.hpp"

SkillEL_WIND_SLASH::SkillEL_WIND_SLASH() : WeaponSkillImpl(EL_WIND_SLASH) {
}

void SkillEL_WIND_SLASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100;
}

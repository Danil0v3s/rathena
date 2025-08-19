#include "skill_el_tidal_weapon.hpp"

SkillEL_TIDAL_WEAPON::SkillEL_TIDAL_WEAPON() : WeaponSkillImpl(EL_TIDAL_WEAPON) {
}

void SkillEL_TIDAL_WEAPON::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 1400;
}

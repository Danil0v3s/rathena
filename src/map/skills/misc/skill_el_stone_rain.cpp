#include "skill_el_stone_rain.hpp"

SkillEL_STONE_RAIN::SkillEL_STONE_RAIN() : WeaponSkillImpl(EL_STONE_RAIN) {
}

void SkillEL_STONE_RAIN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 200;
}

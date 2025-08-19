#include "skill_el_stone_hammer.hpp"

SkillEL_STONE_HAMMER::SkillEL_STONE_HAMMER() : WeaponSkillImpl(EL_STONE_HAMMER) {
}

void SkillEL_STONE_HAMMER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 400;
}

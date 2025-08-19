#include "skill_el_circle_of_fire.hpp"

SkillEL_CIRCLE_OF_FIRE::SkillEL_CIRCLE_OF_FIRE() : WeaponSkillImpl(EL_CIRCLE_OF_FIRE) {
}

void SkillEL_CIRCLE_OF_FIRE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 200;
}

#include "skill_el_rock_crusher.hpp"

SkillEL_ROCK_CRUSHER::SkillEL_ROCK_CRUSHER() : WeaponSkillImpl(EL_ROCK_CRUSHER) {
}

void SkillEL_ROCK_CRUSHER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 700;
}

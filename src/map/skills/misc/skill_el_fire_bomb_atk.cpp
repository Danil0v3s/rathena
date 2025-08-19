#include "skill_el_fire_bomb_atk.hpp"

SkillEL_FIRE_BOMB_ATK::SkillEL_FIRE_BOMB_ATK() : WeaponSkillImpl(EL_FIRE_BOMB_ATK) {
}

void SkillEL_FIRE_BOMB_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 200;
}

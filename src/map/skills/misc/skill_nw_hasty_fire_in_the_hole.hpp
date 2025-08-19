#pragma once

#include "../weapon_skill_impl.hpp"

#include "../../battle.hpp"

class SkillNW_HASTY_FIRE_IN_THE_HOLE : public WeaponSkillImpl {
public:
	SkillNW_HASTY_FIRE_IN_THE_HOLE();
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const override;
};
#pragma once

#include "../weapon_skill_impl.hpp"

#include "../../battle.hpp"

class SkillIG_SHIELD_SHOOTING : public WeaponSkillImpl {
public:
	SkillIG_SHIELD_SHOOTING();
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const override;
};
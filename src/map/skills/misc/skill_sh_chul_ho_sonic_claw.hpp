#pragma once

#include "../weapon_skill_impl.hpp"

#include "../../battle.hpp"

class SkillSH_CHUL_HO_SONIC_CLAW : public WeaponSkillImpl {
public:
	SkillSH_CHUL_HO_SONIC_CLAW();
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const override;
};
#pragma once

#include "../weapon_skill_impl.hpp"

#include "../../battle.hpp"
#include "../../status.hpp"

class SkillHT_POWER : public WeaponSkillImpl {
public:
	SkillHT_POWER();
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const override;
};
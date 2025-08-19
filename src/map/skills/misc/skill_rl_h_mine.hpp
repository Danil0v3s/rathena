#pragma once

#include "../weapon_skill_impl.hpp"

#include "../../battle.hpp"

class SkillRL_H_MINE : public WeaponSkillImpl {
public:
	SkillRL_H_MINE();
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const override;
};
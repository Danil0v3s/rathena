#pragma once

#include "../weapon_skill_impl.hpp"

#include "../../battle.hpp"

class SkillMS_BOWLINGBASH : public WeaponSkillImpl {
public:
	SkillMS_BOWLINGBASH();
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const override;
};
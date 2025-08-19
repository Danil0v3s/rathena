#pragma once

#include "../skill_impl.hpp"

#include "../../battle.hpp"

class SkillNPC_PULSESTRIKE2 : public SkillImpl {
public:
	SkillNPC_PULSESTRIKE2();
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const override;
};
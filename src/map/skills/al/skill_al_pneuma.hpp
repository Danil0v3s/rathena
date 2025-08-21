#pragma once

#include "../skill_impl.hpp"

#include "../../battle.hpp"

class SkillAL_PNEUMA : public SkillImpl {
public:
	SkillAL_PNEUMA();
	
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const override;
	void modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	int32 castendPos2(block_list* src, int16 x, int16 y, uint16 skill_lv, t_tick tick, int32 flag) const override;
};
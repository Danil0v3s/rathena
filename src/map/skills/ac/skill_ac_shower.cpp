#include "skill_ac_shower.hpp"

SkillAC_SHOWER::SkillAC_SHOWER() : WeaponSkillImpl(AC_SHOWER) {
}

void SkillAC_SHOWER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 50 + 10 * skill_lv;
#else
	base_skillratio += -25 + 5 * skill_lv;
#endif
}

void SkillAC_SHOWER::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	// Note: AC_SHOWER is handled in the splash attack section
	// It falls through to the AS_SPLASHER case in skill_castend_damage_id.cpp
	// This is just a placeholder implementation
	skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
}
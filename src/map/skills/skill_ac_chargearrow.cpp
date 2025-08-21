#include "skill_ac_chargearrow.hpp"

SkillAC_CHARGEARROW::SkillAC_CHARGEARROW() : WeaponSkillImpl(AC_CHARGEARROW) {
}

void SkillAC_CHARGEARROW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50;
}

void SkillAC_CHARGEARROW::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
}
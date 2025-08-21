#include "skill_ac_concentration.hpp"

SkillAC_CONCENTRATION::SkillAC_CONCENTRATION() : WeaponSkillImpl(AC_CONCENTRATION) {
}

void SkillAC_CONCENTRATION::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	int32 splash = skill_get_splash(getSkillId(), skill_lv);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, SC_CONCENTRATION, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	skill_reveal_trap_inarea(src, splash, src->x, src->y);
	map_foreachinallrange(status_change_timer_sub, src, splash, BL_CHAR, src, nullptr, SC_CONCENTRATION, tick);
}
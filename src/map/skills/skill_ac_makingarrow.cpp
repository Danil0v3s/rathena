#include "skill_ac_makingarrow.hpp"

SkillAC_MAKINGARROW::SkillAC_MAKINGARROW() : WeaponSkillImpl(AC_MAKINGARROW) {
}

void SkillAC_MAKINGARROW::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	struct map_session_data *sd = BL_CAST(BL_PC, src);
	if (sd != nullptr)
	{
		clif_arrow_create_list(*sd);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}
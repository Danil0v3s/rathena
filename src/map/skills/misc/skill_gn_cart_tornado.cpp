#include "skill_gn_cart_tornado.hpp"
#include "../../pc.hpp"

SkillGN_CART_TORNADO::SkillGN_CART_TORNADO() : WeaponSkillImpl(GN_CART_TORNADO) {
}

void SkillGN_CART_TORNADO::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	// ATK [( Skill Level x 200 ) + ( Cart Weight / ( 150 - Caster Base STR ))] + ( Cart Remodeling Skill Level x 50 )] %
	base_skillratio += -100 + 200 * skill_lv;
	if(src->type == BL_PC && ((map_session_data*)src)->cart_weight)
		base_skillratio += ((map_session_data*)src)->cart_weight / 10 / (150 - min(((map_session_data*)src)->status.str,120)) + pc_checkskill((map_session_data*)src,GN_REMODELING_CART) * 50;
}

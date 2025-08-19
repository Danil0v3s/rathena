#include "skill_ws_carttermination.hpp"

SkillWS_CARTTERMINATION::SkillWS_CARTTERMINATION() : WeaponSkillImpl(WS_CARTTERMINATION) {
}

void SkillWS_CARTTERMINATION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	int32 i = 10 * (16 - skill_lv);
	if (i < 1) i = 1;
	//Preserve damage ratio when max cart weight is changed.
	if (sd && sd->cart_weight)
		base_skillratio += sd->cart_weight / i * 80000 / battle_config.max_cart_weight - 100;
	else if (!sd)
		base_skillratio += 80000 / i - 100;
}

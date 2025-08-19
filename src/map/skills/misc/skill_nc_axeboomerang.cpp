#include "skill_nc_axeboomerang.hpp"

SkillNC_AXEBOOMERANG::SkillNC_AXEBOOMERANG() : WeaponSkillImpl(NC_AXEBOOMERANG) {
}

void SkillNC_AXEBOOMERANG::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 150 + 50 * skill_lv;
	if (sd) {
		int16 index = sd->equip_index[EQI_HAND_R];

		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_WEAPON)
			base_skillratio += sd->inventory_data[index]->weight / 10;// Weight is divided by 10 since 10 weight in coding make 1 whole actual weight. [Rytech]
	}
	RE_LVL_DMOD(100);
}

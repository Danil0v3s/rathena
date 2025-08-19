#include "skill_ig_shield_shooting.hpp"

SkillIG_SHIELD_SHOOTING::SkillIG_SHIELD_SHOOTING() : WeaponSkillImpl(IG_SHIELD_SHOOTING) {
}

void SkillIG_SHIELD_SHOOTING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1000 + 3500 * skill_lv;
	base_skillratio += 10 * sstatus->pow;
	base_skillratio += skill_lv * 150 * pc_checkskill( sd, IG_SHIELD_MASTERY );
	if (sd) { // Damage affected by the shield's weight and refine. Need official formula. [Rytech]
		int16 index = sd->equip_index[EQI_HAND_L];

		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_ARMOR) {
			base_skillratio += (sd->inventory_data[index]->weight * 7 / 6) / 10;
			base_skillratio += sd->inventory.u.items_inventory[index].refine * 100;
		}
	}
	RE_LVL_DMOD(100);
}

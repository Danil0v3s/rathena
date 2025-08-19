#include "skill_pa_shieldchain.hpp"

SkillPA_SHIELDCHAIN::SkillPA_SHIELDCHAIN() : WeaponSkillImpl(PA_SHIELDCHAIN) {
}

void SkillPA_SHIELDCHAIN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio = -100 + 300 + 200 * skill_lv;

	if( sd != nullptr ){
		int16 index = sd->equip_index[EQI_HAND_L];

		// Damage affected by the shield's weight and refine.
		if( index >= 0 && sd->inventory_data[index] != nullptr && sd->inventory_data[index]->type == IT_ARMOR ){
			base_skillratio += sd->inventory_data[index]->weight / 10 + 4 * sd->inventory.u.items_inventory[index].refine;
		}

		// Damage affected by shield mastery
		if( sc != nullptr && sc->getSCE( SC_SHIELD_POWER ) ){
			base_skillratio += skill_lv * 14 * pc_checkskill( sd, IG_SHIELD_MASTERY );
		}
	}

	RE_LVL_DMOD(100);
#else
	base_skillratio += 30 * skill_lv;
#endif
	if (sc && sc->getSCE(SC_SHIELD_POWER))// Whats the official increase? [Rytech]
		base_skillratio += base_skillratio * 50 / 100;
}

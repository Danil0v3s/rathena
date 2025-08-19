#include "skill_lg_shieldpress.hpp"

SkillLG_SHIELDPRESS::SkillLG_SHIELDPRESS() : WeaponSkillImpl(LG_SHIELDPRESS) {
}

void SkillLG_SHIELDPRESS::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 * skill_lv;
	if (sd) {
		// Shield Press only considers base STR without job bonus
		base_skillratio += sd->status.str;

		if( sc != nullptr && sc->getSCE( SC_SHIELD_POWER ) ){
			base_skillratio += skill_lv * 15 * pc_checkskill( sd, IG_SHIELD_MASTERY );
		}

		int16 index = sd->equip_index[EQI_HAND_L];

		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_ARMOR)
			base_skillratio += sd->inventory_data[index]->weight / 10;
	}
	RE_LVL_DMOD(100);
}

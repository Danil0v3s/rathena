#include "skill_dk_madness_crusher.hpp"

SkillDK_MADNESS_CRUSHER::SkillDK_MADNESS_CRUSHER() : WeaponSkillImpl(DK_MADNESS_CRUSHER) {
}

void SkillDK_MADNESS_CRUSHER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1000 + 3800 * skill_lv;
	base_skillratio += 10 * sstatus->pow;
	if( sd != nullptr ){
		int16 index = sd->equip_index[EQI_HAND_R];

		if( index >= 0 && sd->inventory_data[index] != nullptr ){
			base_skillratio += sd->inventory_data[index]->weight / 10 * sd->inventory_data[index]->weapon_level;
		}
	}
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		base_skillratio *= 2;
}

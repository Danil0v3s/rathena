#include "skill_ig_radiant_spear.hpp"

SkillIG_RADIANT_SPEAR::SkillIG_RADIANT_SPEAR() : WeaponSkillImpl(IG_RADIANT_SPEAR) {
}

void SkillIG_RADIANT_SPEAR::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 3500 + 1150 * skill_lv;
	base_skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 50;
	base_skillratio += 5 * sstatus->pow;	// !TODO: check POW ratio

	if( sc != nullptr && sc->getSCE( SC_SPEAR_SCAR ) )
		base_skillratio += 250 * skill_lv;

	RE_LVL_DMOD(100);
}

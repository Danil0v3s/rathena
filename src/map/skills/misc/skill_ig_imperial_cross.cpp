#include "skill_ig_imperial_cross.hpp"

SkillIG_IMPERIAL_CROSS::SkillIG_IMPERIAL_CROSS() : WeaponSkillImpl(IG_IMPERIAL_CROSS) {
}

void SkillIG_IMPERIAL_CROSS::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1650 + 1350 * skill_lv;
	base_skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 25;
	base_skillratio += 5 * sstatus->pow;	// !TODO: check POW ratio

	if( sc != nullptr && sc->getSCE( SC_SPEAR_SCAR ) )
		base_skillratio += 100 + 300 * skill_lv;

	RE_LVL_DMOD(100);
}

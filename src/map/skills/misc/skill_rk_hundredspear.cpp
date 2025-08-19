#include "skill_rk_hundredspear.hpp"

SkillRK_HUNDREDSPEAR::SkillRK_HUNDREDSPEAR() : WeaponSkillImpl(RK_HUNDREDSPEAR) {
}

void SkillRK_HUNDREDSPEAR::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 600 + 200 * skill_lv;
	if (sd)
		base_skillratio += 50 * pc_checkskill(sd,LK_SPIRALPIERCE);
	if (sc) {
		if( sc->getSCE( SC_DRAGONIC_AURA ) ){
			base_skillratio += sc->getSCE( SC_DRAGONIC_AURA )->val1 * 160;
		}

		if (sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
			base_skillratio *= 2;
	}
	RE_LVL_DMOD(100);
}

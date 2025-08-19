#include "skill_lk_spiralpierce.hpp"

SkillLK_SPIRALPIERCE::SkillLK_SPIRALPIERCE() : WeaponSkillImpl(LK_SPIRALPIERCE) {
}

#ifdef RENEWAL
void SkillLK_SPIRALPIERCE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50 + 50 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		base_skillratio *= 2;
}
#endif
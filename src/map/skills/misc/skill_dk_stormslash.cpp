#include "skill_dk_stormslash.hpp"

SkillDK_STORMSLASH::SkillDK_STORMSLASH() : WeaponSkillImpl(DK_STORMSLASH) {
}

void SkillDK_STORMSLASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 300 + 750 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_GIANTGROWTH) && rnd_chance(60, 100))
		base_skillratio *= 2;
}

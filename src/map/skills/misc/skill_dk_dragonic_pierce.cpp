#include "skill_dk_dragonic_pierce.hpp"

SkillDK_DRAGONIC_PIERCE::SkillDK_DRAGONIC_PIERCE() : WeaponSkillImpl(DK_DRAGONIC_PIERCE) {
}

void SkillDK_DRAGONIC_PIERCE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 850 + 600 * skill_lv;
	base_skillratio += 7 * sstatus->pow;	// !TODO: unknown ratio

	if (sc != nullptr && sc->hasSCE(SC_DRAGONIC_AURA))
		base_skillratio += 100 + 50 * skill_lv;

	RE_LVL_DMOD(100);
}

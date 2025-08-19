#include "skill_dk_dragonic_breath.hpp"

SkillDK_DRAGONIC_BREATH::SkillDK_DRAGONIC_BREATH() : WeaponSkillImpl(DK_DRAGONIC_BREATH) {
}

void SkillDK_DRAGONIC_BREATH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 50 + 350 * skill_lv;
	base_skillratio += 7 * sstatus->pow;

	if (sc && sc->getSCE(SC_DRAGONIC_AURA)) {
		base_skillratio += 3 * sstatus->pow;
		base_skillratio += (skill_lv * (sstatus->max_hp * 25 / 100) * 7) / 100;	// Skill level x 0.07 x ((MaxHP / 4) + (MaxSP / 2))
		base_skillratio += (skill_lv * (sstatus->max_sp * 50 / 100) * 7) / 100;
	} else {
		base_skillratio += (skill_lv * (sstatus->max_hp * 25 / 100) * 5) / 100;	// Skill level x 0.05 x ((MaxHP / 4) + (MaxSP / 2))
		base_skillratio += (skill_lv * (sstatus->max_sp * 50 / 100) * 5) / 100;
	}

	RE_LVL_DMOD(100);
}

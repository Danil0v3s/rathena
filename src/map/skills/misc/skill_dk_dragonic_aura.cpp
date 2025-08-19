#include "skill_dk_dragonic_aura.hpp"

SkillDK_DRAGONIC_AURA::SkillDK_DRAGONIC_AURA() : WeaponSkillImpl(DK_DRAGONIC_AURA) {
}

void SkillDK_DRAGONIC_AURA::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 3650 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_ANGEL)
		base_skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

#include "skill_sr_rideinlightning.hpp"

SkillSR_RIDEINLIGHTNING::SkillSR_RIDEINLIGHTNING() : WeaponSkillImpl(SR_RIDEINLIGHTNING) {
}

void SkillSR_RIDEINLIGHTNING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 40 * skill_lv;
	if (sd && sd->status.weapon == W_KNUCKLE)
		base_skillratio += 50 * skill_lv;
	RE_LVL_DMOD(100);
}

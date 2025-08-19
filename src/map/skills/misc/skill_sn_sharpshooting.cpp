#include "skill_sn_sharpshooting.hpp"

SkillSN_SHARPSHOOTING::SkillSN_SHARPSHOOTING() : WeaponSkillImpl(SN_SHARPSHOOTING) {
}

void SkillSN_SHARPSHOOTING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (src->type == BL_MOB) { // TODO: Did these formulas change in the renewal balancing?
		if (wd->miscflag & 2) // Splash damage bonus
			base_skillratio += -100 + 140 * skill_lv;
		else
			base_skillratio += 100 + 50 * skill_lv;
		return;
	}
	// Fallthrough to MA_SHARPSHOOTING logic
}

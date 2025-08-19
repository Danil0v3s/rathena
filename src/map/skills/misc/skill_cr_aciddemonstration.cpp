#include "skill_cr_aciddemonstration.hpp"

SkillCR_ACIDDEMONSTRATION::SkillCR_ACIDDEMONSTRATION() : WeaponSkillImpl(CR_ACIDDEMONSTRATION) {
}

void SkillCR_ACIDDEMONSTRATION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + 200 * skill_lv + sstatus->int_ + tstatus->vit; // !TODO: Confirm status bonus
	if (target->type == BL_PC)
		base_skillratio /= 2;
#endif
}

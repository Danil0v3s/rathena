#include "skill_shc_shadow_stab.hpp"

SkillSHC_SHADOW_STAB::SkillSHC_SHADOW_STAB() : WeaponSkillImpl(SHC_SHADOW_STAB) {
}

void SkillSHC_SHADOW_STAB::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 550 * skill_lv;
	base_skillratio += 5 * sstatus->pow;

	if (wd->miscflag & SKILL_ALTDMG_FLAG) {
		base_skillratio += 100 * skill_lv + 2 * sstatus->pow;
	}

	RE_LVL_DMOD(100);
}

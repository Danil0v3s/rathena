#include "skill_asc_breaker.hpp"

SkillASC_BREAKER::SkillASC_BREAKER() : WeaponSkillImpl(ASC_BREAKER) {
}

void SkillASC_BREAKER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + 150 * skill_lv + sstatus->str + sstatus->int_; // !TODO: Confirm stat modifier
	RE_LVL_DMOD(100);
#else
	// Pre-Renewal: skill ratio for weapon part of damage [helvetica]
	base_skillratio += -100 + 100 * skill_lv;
#endif
}

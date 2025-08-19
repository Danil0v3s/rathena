#include "skill_asc_meteorassault.hpp"

SkillASC_METEORASSAULT::SkillASC_METEORASSAULT() : WeaponSkillImpl(ASC_METEORASSAULT) {
}

void SkillASC_METEORASSAULT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 100 + 120 * skill_lv;
	RE_LVL_DMOD(100);
#else
	base_skillratio += -60 + 40 * skill_lv;
#endif
}

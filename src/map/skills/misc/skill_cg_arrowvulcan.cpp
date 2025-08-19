#include "skill_cg_arrowvulcan.hpp"

SkillCG_ARROWVULCAN::SkillCG_ARROWVULCAN() : WeaponSkillImpl(CG_ARROWVULCAN) {
}

void SkillCG_ARROWVULCAN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 400 + 100 * skill_lv;
	RE_LVL_DMOD(100);
#else
	base_skillratio += 100 + 100 * skill_lv;
#endif
}

#include "skill_ma_sharpshooting.hpp"

SkillMA_SHARPSHOOTING::SkillMA_SHARPSHOOTING() : WeaponSkillImpl(MA_SHARPSHOOTING) {
}

void SkillMA_SHARPSHOOTING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + 300 + 300 * skill_lv;
	RE_LVL_DMOD(100);
#else
	base_skillratio += 100 + 50 * skill_lv;
#endif
}

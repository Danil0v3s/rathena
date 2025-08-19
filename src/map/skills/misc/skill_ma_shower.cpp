#include "skill_ma_shower.hpp"

SkillMA_SHOWER::SkillMA_SHOWER() : WeaponSkillImpl(MA_SHOWER) {
}

void SkillMA_SHOWER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 50 + 10 * skill_lv;
#else
	base_skillratio += -25 + 5 * skill_lv;
#endif
}

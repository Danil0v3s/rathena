#include "skill_gs_spreadattack.hpp"

SkillGS_SPREADATTACK::SkillGS_SPREADATTACK() : WeaponSkillImpl(GS_SPREADATTACK) {
}

void SkillGS_SPREADATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 30 * skill_lv;
#else
	base_skillratio += 20 * (skill_lv - 1);
#endif
}

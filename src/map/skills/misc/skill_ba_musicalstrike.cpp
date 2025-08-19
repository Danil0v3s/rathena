#include "skill_ba_musicalstrike.hpp"

SkillBA_MUSICALSTRIKE::SkillBA_MUSICALSTRIKE() : WeaponSkillImpl(BA_MUSICALSTRIKE) {
}

void SkillBA_MUSICALSTRIKE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 10 + 40 * skill_lv;
#else
	base_skillratio += 25 + 25 * skill_lv;
#endif
}

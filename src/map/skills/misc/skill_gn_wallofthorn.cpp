#include "skill_gn_wallofthorn.hpp"

SkillGN_WALLOFTHORN::SkillGN_WALLOFTHORN() : WeaponSkillImpl(GN_WALLOFTHORN) {
}

void SkillGN_WALLOFTHORN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 10 * skill_lv;
}

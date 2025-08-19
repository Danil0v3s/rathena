#include "skill_abr_dual_cannon_fire.hpp"

SkillABR_DUAL_CANNON_FIRE::SkillABR_DUAL_CANNON_FIRE() : WeaponSkillImpl(ABR_DUAL_CANNON_FIRE) {
}

void SkillABR_DUAL_CANNON_FIRE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 8000;
}

#include "skill_abr_infinity_buster.hpp"

SkillABR_INFINITY_BUSTER::SkillABR_INFINITY_BUSTER() : WeaponSkillImpl(ABR_INFINITY_BUSTER) {
}

void SkillABR_INFINITY_BUSTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 50000;
}

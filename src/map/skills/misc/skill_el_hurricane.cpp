#include "skill_el_hurricane.hpp"

SkillEL_HURRICANE::SkillEL_HURRICANE() : WeaponSkillImpl(EL_HURRICANE) {
}

void SkillEL_HURRICANE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 600;
}

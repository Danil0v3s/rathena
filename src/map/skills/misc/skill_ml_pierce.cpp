#include "skill_ml_pierce.hpp"

SkillML_PIERCE::SkillML_PIERCE() : WeaponSkillImpl(ML_PIERCE) {
}

void SkillML_PIERCE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 10 * skill_lv;
}

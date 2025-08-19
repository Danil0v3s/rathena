#include "skill_tf_sprinklesand.hpp"

SkillTF_SPRINKLESAND::SkillTF_SPRINKLESAND() : WeaponSkillImpl(TF_SPRINKLESAND) {
}

void SkillTF_SPRINKLESAND::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 30;
}

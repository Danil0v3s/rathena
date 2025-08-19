#include "skill_as_grimtooth.hpp"

SkillAS_GRIMTOOTH::SkillAS_GRIMTOOTH() : WeaponSkillImpl(AS_GRIMTOOTH) {
}

void SkillAS_GRIMTOOTH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 20 * skill_lv;
}

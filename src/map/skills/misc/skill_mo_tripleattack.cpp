#include "skill_mo_tripleattack.hpp"

SkillMO_TRIPLEATTACK::SkillMO_TRIPLEATTACK() : WeaponSkillImpl(MO_TRIPLEATTACK) {
}

void SkillMO_TRIPLEATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 20 * skill_lv;
}

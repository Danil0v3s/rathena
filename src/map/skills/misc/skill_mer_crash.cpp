#include "skill_mer_crash.hpp"

SkillMER_CRASH::SkillMER_CRASH() : WeaponSkillImpl(MER_CRASH) {
}

void SkillMER_CRASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 10 * skill_lv;
}

#include "skill_ko_makibishi.hpp"

SkillKO_MAKIBISHI::SkillKO_MAKIBISHI() : WeaponSkillImpl(KO_MAKIBISHI) {
}

void SkillKO_MAKIBISHI::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 20 * skill_lv;
}

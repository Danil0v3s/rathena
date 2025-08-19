#include "skill_pa_sacrifice.hpp"

SkillPA_SACRIFICE::SkillPA_SACRIFICE() : WeaponSkillImpl(PA_SACRIFICE) {
}

void SkillPA_SACRIFICE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -10 + 10 * skill_lv;
}

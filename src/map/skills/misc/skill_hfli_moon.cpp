#include "skill_hfli_moon.hpp"

SkillHFLI_MOON::SkillHFLI_MOON() : WeaponSkillImpl(HFLI_MOON) {
}

void SkillHFLI_MOON::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 10 + 110 * skill_lv;
}

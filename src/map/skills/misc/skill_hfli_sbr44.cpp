#include "skill_hfli_sbr44.hpp"

SkillHFLI_SBR44::SkillHFLI_SBR44() : WeaponSkillImpl(HFLI_SBR44) {
}

void SkillHFLI_SBR44::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

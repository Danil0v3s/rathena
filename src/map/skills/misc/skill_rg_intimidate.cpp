#include "skill_rg_intimidate.hpp"

SkillRG_INTIMIDATE::SkillRG_INTIMIDATE() : WeaponSkillImpl(RG_INTIMIDATE) {
}

void SkillRG_INTIMIDATE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 30 * skill_lv;
}

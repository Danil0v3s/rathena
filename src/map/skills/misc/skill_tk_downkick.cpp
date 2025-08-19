#include "skill_tk_downkick.hpp"

SkillTK_DOWNKICK::SkillTK_DOWNKICK() : WeaponSkillImpl(TK_DOWNKICK) {
}

void SkillTK_DOWNKICK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 60 + 20 * skill_lv;
}

#include "skill_ma_double.hpp"

SkillMA_DOUBLE::SkillMA_DOUBLE() : WeaponSkillImpl(MA_DOUBLE) {
}

void SkillMA_DOUBLE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 10 * (skill_lv - 1);
}

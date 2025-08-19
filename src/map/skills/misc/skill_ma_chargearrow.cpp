#include "skill_ma_chargearrow.hpp"

SkillMA_CHARGEARROW::SkillMA_CHARGEARROW() : WeaponSkillImpl(MA_CHARGEARROW) {
}

void SkillMA_CHARGEARROW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50;
}

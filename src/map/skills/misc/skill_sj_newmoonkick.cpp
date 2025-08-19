#include "skill_sj_newmoonkick.hpp"

SkillSJ_NEWMOONKICK::SkillSJ_NEWMOONKICK() : WeaponSkillImpl(SJ_NEWMOONKICK) {
}

void SkillSJ_NEWMOONKICK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 600 + 100 * skill_lv;
}

#include "skill_sj_staremperor.hpp"

SkillSJ_STAREMPEROR::SkillSJ_STAREMPEROR() : WeaponSkillImpl(SJ_STAREMPEROR) {
}

void SkillSJ_STAREMPEROR::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 700 + 200 * skill_lv;
}

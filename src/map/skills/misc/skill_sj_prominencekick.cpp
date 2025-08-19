#include "skill_sj_prominencekick.hpp"

SkillSJ_PROMINENCEKICK::SkillSJ_PROMINENCEKICK() : WeaponSkillImpl(SJ_PROMINENCEKICK) {
}

void SkillSJ_PROMINENCEKICK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50 + 50 * skill_lv;
}

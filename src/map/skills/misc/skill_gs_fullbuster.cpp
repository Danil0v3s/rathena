#include "skill_gs_fullbuster.hpp"

SkillGS_FULLBUSTER::SkillGS_FULLBUSTER() : WeaponSkillImpl(GS_FULLBUSTER) {
}

void SkillGS_FULLBUSTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv + 2);
}

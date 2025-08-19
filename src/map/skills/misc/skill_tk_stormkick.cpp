#include "skill_tk_stormkick.hpp"

SkillTK_STORMKICK::SkillTK_STORMKICK() : WeaponSkillImpl(TK_STORMKICK) {
}

void SkillTK_STORMKICK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 60 + 20 * skill_lv;
}

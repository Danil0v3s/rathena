#include "skill_tk_turnkick.hpp"

SkillTK_TURNKICK::SkillTK_TURNKICK() : WeaponSkillImpl(TK_TURNKICK) {
}

void SkillTK_TURNKICK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 90 + 30 * skill_lv;
}

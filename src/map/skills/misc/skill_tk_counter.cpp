#include "skill_tk_counter.hpp"

SkillTK_COUNTER::SkillTK_COUNTER() : WeaponSkillImpl(TK_COUNTER) {
}

void SkillTK_COUNTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 90 + 30 * skill_lv;
}

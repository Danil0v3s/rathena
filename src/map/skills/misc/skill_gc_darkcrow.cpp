#include "skill_gc_darkcrow.hpp"

SkillGC_DARKCROW::SkillGC_DARKCROW() : WeaponSkillImpl(GC_DARKCROW) {
}

void SkillGC_DARKCROW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

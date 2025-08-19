#include "skill_gc_phantommenace.hpp"

SkillGC_PHANTOMMENACE::SkillGC_PHANTOMMENACE() : WeaponSkillImpl(GC_PHANTOMMENACE) {
}

void SkillGC_PHANTOMMENACE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 200;
}

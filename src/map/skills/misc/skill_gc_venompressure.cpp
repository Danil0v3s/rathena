#include "skill_gc_venompressure.hpp"

SkillGC_VENOMPRESSURE::SkillGC_VENOMPRESSURE() : WeaponSkillImpl(GC_VENOMPRESSURE) {
}

void SkillGC_VENOMPRESSURE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 900;
}

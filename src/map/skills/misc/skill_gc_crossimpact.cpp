#include "skill_gc_crossimpact.hpp"

SkillGC_CROSSIMPACT::SkillGC_CROSSIMPACT() : WeaponSkillImpl(GC_CROSSIMPACT) {
}

void SkillGC_CROSSIMPACT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1400 + 150 * skill_lv;
	RE_LVL_DMOD(100);
}

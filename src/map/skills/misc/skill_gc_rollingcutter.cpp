#include "skill_gc_rollingcutter.hpp"

SkillGC_ROLLINGCUTTER::SkillGC_ROLLINGCUTTER() : WeaponSkillImpl(GC_ROLLINGCUTTER) {
}

void SkillGC_ROLLINGCUTTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 50 + 80 * skill_lv;
	RE_LVL_DMOD(100);
}

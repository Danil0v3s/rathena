#include "skill_gc_crossripperslasher.hpp"

SkillGC_CROSSRIPPERSLASHER::SkillGC_CROSSRIPPERSLASHER() : WeaponSkillImpl(GC_CROSSRIPPERSLASHER) {
}

void SkillGC_CROSSRIPPERSLASHER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 80 * skill_lv + (sstatus->agi * 3);
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_ROLLINGCUTTER))
		base_skillratio += sc->getSCE(SC_ROLLINGCUTTER)->val1 * 200;
}

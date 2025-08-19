#include "skill_gc_counterslash.hpp"

SkillGC_COUNTERSLASH::SkillGC_COUNTERSLASH() : WeaponSkillImpl(GC_COUNTERSLASH) {
}

void SkillGC_COUNTERSLASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	//ATK [{(Skill Level x 150) + 300} x Caster's Base Level / 120]% + ATK [(AGI x 2) + (Caster's Job Level x 4)]%
	base_skillratio += -100 + 300 + 150 * skill_lv;
	RE_LVL_DMOD(120);
	base_skillratio += sstatus->agi * 2;
	// If 4th job, job level of your 3rd job counts
	base_skillratio += (sd ? (sd->class_&JOBL_FOURTH ? sd->change_level_4th : sd->status.job_level) * 4 : 0);
}

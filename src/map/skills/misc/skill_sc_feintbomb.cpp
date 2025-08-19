#include "skill_sc_feintbomb.hpp"

SkillSC_FEINTBOMB::SkillSC_FEINTBOMB() : WeaponSkillImpl(SC_FEINTBOMB) {
}

void SkillSC_FEINTBOMB::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + (skill_lv + 1) * sstatus->dex / 2 * ((sd) ? sd->status.job_level / 10 : 1);
	RE_LVL_DMOD(120);
}

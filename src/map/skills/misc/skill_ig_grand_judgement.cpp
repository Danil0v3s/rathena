#include "skill_ig_grand_judgement.hpp"

SkillIG_GRAND_JUDGEMENT::SkillIG_GRAND_JUDGEMENT() : WeaponSkillImpl(IG_GRAND_JUDGEMENT) {
}

void SkillIG_GRAND_JUDGEMENT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 + 1500 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_PLANT || tstatus->race == RC_INSECT)
		base_skillratio += 100 + 150 * skill_lv;
	RE_LVL_DMOD(100);
	if ((i = pc_checkskill_imperial_guard(sd, 3)) > 0)
		base_skillratio += base_skillratio * i / 100;
}

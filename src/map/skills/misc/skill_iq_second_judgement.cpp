#include "skill_iq_second_judgement.hpp"

SkillIQ_SECOND_JUDGEMENT::SkillIQ_SECOND_JUDGEMENT() : WeaponSkillImpl(IQ_SECOND_JUDGEMENT) {
}

void SkillIQ_SECOND_JUDGEMENT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 150 + 2600 * skill_lv + 7 * sstatus->pow;
	RE_LVL_DMOD(100);
}

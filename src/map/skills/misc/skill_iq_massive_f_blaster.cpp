#include "skill_iq_massive_f_blaster.hpp"

SkillIQ_MASSIVE_F_BLASTER::SkillIQ_MASSIVE_F_BLASTER() : WeaponSkillImpl(IQ_MASSIVE_F_BLASTER) {
}

void SkillIQ_MASSIVE_F_BLASTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 2300 * skill_lv + 15 * sstatus->pow;
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_DEMON)
		base_skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

#include "skill_wh_hawkboomerang.hpp"

SkillWH_HAWKBOOMERANG::SkillWH_HAWKBOOMERANG() : WeaponSkillImpl(WH_HAWKBOOMERANG) {
}

void SkillWH_HAWKBOOMERANG::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 600 * skill_lv + 10 * sstatus->con;
	if (sd)
		base_skillratio += base_skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH)
		base_skillratio += base_skillratio * 50 / 100;
	RE_LVL_DMOD(100);
}

#include "skill_wh_hawkrush.hpp"

SkillWH_HAWKRUSH::SkillWH_HAWKRUSH() : WeaponSkillImpl(WH_HAWKRUSH) {
}

void SkillWH_HAWKRUSH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 500 * skill_lv + 5 * sstatus->con;
	if (sd)
		base_skillratio += base_skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	RE_LVL_DMOD(100);
}

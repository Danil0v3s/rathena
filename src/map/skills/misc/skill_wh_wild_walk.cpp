#include "skill_wh_wild_walk.hpp"

SkillWH_WILD_WALK::SkillWH_WILD_WALK() : WeaponSkillImpl(WH_WILD_WALK) {
}

void SkillWH_WILD_WALK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1800 + 2800 * skill_lv;
	// !TODO: unknown con and WH_NATUREFRIENDLY/HT_STEELCROW skills ratio
	base_skillratio += 5 * sstatus->con;
	base_skillratio += base_skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	base_skillratio += base_skillratio * pc_checkskill(sd, HT_STEELCROW) / 10;
	RE_LVL_DMOD(100);
}

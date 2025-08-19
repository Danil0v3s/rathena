#include "skill_nw_hasty_fire_in_the_hole.hpp"

SkillNW_HASTY_FIRE_IN_THE_HOLE::SkillNW_HASTY_FIRE_IN_THE_HOLE() : WeaponSkillImpl(NW_HASTY_FIRE_IN_THE_HOLE) {
}

void SkillNW_HASTY_FIRE_IN_THE_HOLE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1500 + 1500 * skill_lv;
	base_skillratio += pc_checkskill( sd, NW_GRENADE_MASTERY ) * 20;
	base_skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

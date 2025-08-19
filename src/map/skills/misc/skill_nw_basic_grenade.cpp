#include "skill_nw_basic_grenade.hpp"

SkillNW_BASIC_GRENADE::SkillNW_BASIC_GRENADE() : WeaponSkillImpl(NW_BASIC_GRENADE) {
}

void SkillNW_BASIC_GRENADE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1500 + 2100 * skill_lv;
	base_skillratio += pc_checkskill( sd, NW_GRENADE_MASTERY ) * 50;
	base_skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

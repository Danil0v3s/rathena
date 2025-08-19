#include "skill_nw_spiral_shooting.hpp"

SkillNW_SPIRAL_SHOOTING::SkillNW_SPIRAL_SHOOTING() : WeaponSkillImpl(NW_SPIRAL_SHOOTING) {
}

void SkillNW_SPIRAL_SHOOTING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1200 + 1700 * skill_lv;
	base_skillratio += pc_checkskill( sd, NW_GRENADE_MASTERY ) * 30;
	base_skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

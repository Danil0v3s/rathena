#include "skill_mt_rush_quake.hpp"

SkillMT_RUSH_QUAKE::SkillMT_RUSH_QUAKE() : WeaponSkillImpl(MT_RUSH_QUAKE) {
}

void SkillMT_RUSH_QUAKE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 3600 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_FORMLESS || tstatus->race == RC_INSECT)
		base_skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

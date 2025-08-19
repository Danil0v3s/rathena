#include "skill_nw_grenades_dropping.hpp"

SkillNW_GRENADES_DROPPING::SkillNW_GRENADES_DROPPING() : WeaponSkillImpl(NW_GRENADES_DROPPING) {
}

void SkillNW_GRENADES_DROPPING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 550 + 850 * skill_lv;
	base_skillratio += pc_checkskill( sd, NW_GRENADE_MASTERY ) * 30;
	base_skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

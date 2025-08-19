#include "skill_wh_swifttrap.hpp"

SkillWH_SWIFTTRAP::SkillWH_SWIFTTRAP() : WeaponSkillImpl(WH_SWIFTTRAP) {
}

void SkillWH_SWIFTTRAP::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 850 * skill_lv + 5 * sstatus->con;
	RE_LVL_DMOD(100);
	base_skillratio += base_skillratio * (20 * (sd ? pc_checkskill(sd, WH_ADVANCED_TRAP) : 5)) / 100;
}

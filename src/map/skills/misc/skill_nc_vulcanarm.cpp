#include "skill_nc_vulcanarm.hpp"

SkillNC_VULCANARM::SkillNC_VULCANARM() : WeaponSkillImpl(NC_VULCANARM) {
}

void SkillNC_VULCANARM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 230 * skill_lv + sstatus->dex; // !TODO: What's the DEX bonus?
	RE_LVL_DMOD(100);
}

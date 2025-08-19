#include "skill_nc_flamelauncher.hpp"

SkillNC_FLAMELAUNCHER::SkillNC_FLAMELAUNCHER() : WeaponSkillImpl(NC_FLAMELAUNCHER) {
}

void SkillNC_FLAMELAUNCHER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 200 + 300 * skill_lv;
	RE_LVL_DMOD(150);
}

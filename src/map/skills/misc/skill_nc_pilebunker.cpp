#include "skill_nc_pilebunker.hpp"

SkillNC_PILEBUNKER::SkillNC_PILEBUNKER() : WeaponSkillImpl(NC_PILEBUNKER) {
}

void SkillNC_PILEBUNKER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 200 + 100 * skill_lv + status_get_str(src);
	RE_LVL_DMOD(100);
}

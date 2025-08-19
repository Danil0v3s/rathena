#include "skill_ra_clusterbomb.hpp"

SkillRA_CLUSTERBOMB::SkillRA_CLUSTERBOMB() : SkillImpl(RA_CLUSTERBOMB) {
}

void SkillRA_CLUSTERBOMB::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 + 100 * skill_lv;
}

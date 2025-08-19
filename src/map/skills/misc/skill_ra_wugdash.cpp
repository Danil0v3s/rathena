#include "skill_ra_wugdash.hpp"

SkillRA_WUGDASH::SkillRA_WUGDASH() : WeaponSkillImpl(RA_WUGDASH) {
}

void SkillRA_WUGDASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 200; // ATK 300%
}

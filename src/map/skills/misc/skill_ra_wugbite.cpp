#include "skill_ra_wugbite.hpp"

SkillRA_WUGBITE::SkillRA_WUGBITE() : WeaponSkillImpl(RA_WUGBITE) {
}

void SkillRA_WUGBITE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 300 + 200 * skill_lv;
	if (skill_lv == 5)
		base_skillratio += 100;
}

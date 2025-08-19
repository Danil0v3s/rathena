#include "skill_ra_wugstrike.hpp"

SkillRA_WUGSTRIKE::SkillRA_WUGSTRIKE() : WeaponSkillImpl(RA_WUGSTRIKE) {
}

void SkillRA_WUGSTRIKE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 * skill_lv;
}

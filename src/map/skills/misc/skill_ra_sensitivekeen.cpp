#include "skill_ra_sensitivekeen.hpp"

SkillRA_SENSITIVEKEEN::SkillRA_SENSITIVEKEEN() : WeaponSkillImpl(RA_SENSITIVEKEEN) {
}

void SkillRA_SENSITIVEKEEN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50 * skill_lv;
}

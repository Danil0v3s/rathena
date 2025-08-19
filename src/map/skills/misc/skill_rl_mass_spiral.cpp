#include "skill_rl_mass_spiral.hpp"

SkillRL_MASS_SPIRAL::SkillRL_MASS_SPIRAL() : WeaponSkillImpl(RL_MASS_SPIRAL) {
}

void SkillRL_MASS_SPIRAL::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 * skill_lv;
}

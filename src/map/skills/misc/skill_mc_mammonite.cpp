#include "skill_mc_mammonite.hpp"

SkillMC_MAMMONITE::SkillMC_MAMMONITE() : WeaponSkillImpl(MC_MAMMONITE) {
}

void SkillMC_MAMMONITE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50 * skill_lv;
}

#include "skill_kn_spearboomerang.hpp"

SkillKN_SPEARBOOMERANG::SkillKN_SPEARBOOMERANG() : WeaponSkillImpl(KN_SPEARBOOMERANG) {
}

void SkillKN_SPEARBOOMERANG::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50 * skill_lv;
}

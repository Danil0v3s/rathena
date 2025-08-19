#include "skill_kn_spearstab.hpp"

SkillKN_SPEARSTAB::SkillKN_SPEARSTAB() : WeaponSkillImpl(KN_SPEARSTAB) {
}

void SkillKN_SPEARSTAB::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 20 * skill_lv;
}

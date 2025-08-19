#include "skill_su_pickypeck.hpp"

SkillSU_PICKYPECK::SkillSU_PICKYPECK() : WeaponSkillImpl(SU_PICKYPECK) {
}

void SkillSU_PICKYPECK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 + 100 * skill_lv;
	if (status_get_hp(target) < (status_get_max_hp(target) / 2))
		base_skillratio *= 2;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		base_skillratio += base_skillratio * status_get_hp(src) / status_get_max_hp(src);
}

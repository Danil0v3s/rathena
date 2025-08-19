#include "skill_su_scaroftarou.hpp"

SkillSU_SCAROFTAROU::SkillSU_SCAROFTAROU() : WeaponSkillImpl(SU_SCAROFTAROU) {
}

void SkillSU_SCAROFTAROU::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 100 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		base_skillratio += base_skillratio * status_get_hp(src) / status_get_max_hp(src);
}

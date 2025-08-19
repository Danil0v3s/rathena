#include "skill_gs_piercingshot.hpp"

SkillGS_PIERCINGSHOT::SkillGS_PIERCINGSHOT() : WeaponSkillImpl(GS_PIERCINGSHOT) {
}

void SkillGS_PIERCINGSHOT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	if (sd && sd->weapontype1 == W_RIFLE)
		base_skillratio += 150 + 30 * skill_lv;
	else
		base_skillratio += 100 + 20 * skill_lv;
#else
	base_skillratio += 20 * skill_lv;
#endif
}

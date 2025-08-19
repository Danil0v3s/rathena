#include "skill_cr_holycross.hpp"

SkillCR_HOLYCROSS::SkillCR_HOLYCROSS() : WeaponSkillImpl(CR_HOLYCROSS) {
}

void SkillCR_HOLYCROSS::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	// Note: sd (player data) check would need to be implemented separately
	// if(sd && sd->status.weapon == W_2HSPEAR)
	//     base_skillratio += 70 * skill_lv;
	// else
#endif
	base_skillratio += 35 * skill_lv;
}

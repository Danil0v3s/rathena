#include "skill_am_acidterror.hpp"

SkillAM_ACIDTERROR::SkillAM_ACIDTERROR() : WeaponSkillImpl(AM_ACIDTERROR) {
}

void SkillAM_ACIDTERROR::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + 200 * skill_lv;
	if (sd && pc_checkskill(sd, AM_LEARNINGPOTION))
	    base_skillratio += 100; // !TODO: What's this bonus increase?
#else
	base_skillratio += -50 + 50 * skill_lv;
#endif
}

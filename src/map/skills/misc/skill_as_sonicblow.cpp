#include "skill_as_sonicblow.hpp"

SkillAS_SONICBLOW::SkillAS_SONICBLOW() : WeaponSkillImpl(AS_SONICBLOW) {
}

void SkillAS_SONICBLOW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 100 + 100 * skill_lv;
	if (tstatus->hp < (tstatus->max_hp / 2))
		base_skillratio += base_skillratio / 2;
#else
	base_skillratio += 200 + 50 * skill_lv;
	if (sd && pc_checkskill(sd, AS_SONICACCEL) > 0)
	    base_skillratio += base_skillratio / 10;
#endif
}

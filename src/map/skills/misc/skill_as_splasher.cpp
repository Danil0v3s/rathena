#include "skill_as_splasher.hpp"

SkillAS_SPLASHER::SkillAS_SPLASHER() : WeaponSkillImpl(AS_SPLASHER) {
}

void SkillAS_SPLASHER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + 400 + 100 * skill_lv;
#else
	base_skillratio += 400 + 50 * skill_lv;
#endif
	if(sd)
		base_skillratio += 20 * pc_checkskill(sd,AS_POISONREACT);
}

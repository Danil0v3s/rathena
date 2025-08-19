#include "skill_rg_raid.hpp"

SkillRG_RAID::SkillRG_RAID() : WeaponSkillImpl(RG_RAID) {
}

void SkillRG_RAID::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += -100 + 50 + skill_lv * 150;
#else
	base_skillratio += 40 * skill_lv;
#endif
}

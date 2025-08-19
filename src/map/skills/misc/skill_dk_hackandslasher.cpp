#include "skill_dk_hackandslasher.hpp"

SkillDK_HACKANDSLASHER::SkillDK_HACKANDSLASHER() : WeaponSkillImpl(DK_HACKANDSLASHER) {
}

void SkillDK_HACKANDSLASHER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 350 + 820 * skill_lv;
	base_skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
}

#include "skill_ske_rising_moon.hpp"

SkillSKE_RISING_MOON::SkillSKE_RISING_MOON() : WeaponSkillImpl(SKE_RISING_MOON) {
}

void SkillSKE_RISING_MOON::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 700 + 450 * skill_lv;
	base_skillratio += pc_checkskill( sd, SKE_SKY_MASTERY ) * 5 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

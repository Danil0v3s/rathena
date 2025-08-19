#include "skill_rk_sonicwave.hpp"

SkillRK_SONICWAVE::SkillRK_SONICWAVE() : WeaponSkillImpl(RK_SONICWAVE) {
}

void SkillRK_SONICWAVE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1050 + 150 * skill_lv;
	RE_LVL_DMOD(100);
}

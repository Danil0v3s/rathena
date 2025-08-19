#include "skill_rk_ignitionbreak.hpp"

SkillRK_IGNITIONBREAK::SkillRK_IGNITIONBREAK() : WeaponSkillImpl(RK_IGNITIONBREAK) {
}

void SkillRK_IGNITIONBREAK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 450 * skill_lv;
	RE_LVL_DMOD(100);
}

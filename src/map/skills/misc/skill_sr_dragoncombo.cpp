#include "skill_sr_dragoncombo.hpp"

SkillSR_DRAGONCOMBO::SkillSR_DRAGONCOMBO() : WeaponSkillImpl(SR_DRAGONCOMBO) {
}

void SkillSR_DRAGONCOMBO::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 + 80 * skill_lv;
	RE_LVL_DMOD(100);
}

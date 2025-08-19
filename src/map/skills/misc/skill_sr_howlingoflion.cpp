#include "skill_sr_howlingoflion.hpp"

SkillSR_HOWLINGOFLION::SkillSR_HOWLINGOFLION() : WeaponSkillImpl(SR_HOWLINGOFLION) {
}

void SkillSR_HOWLINGOFLION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 500 * skill_lv;
	RE_LVL_DMOD(100);	
}

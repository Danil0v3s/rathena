#include "skill_sr_fallenempire.hpp"

SkillSR_FALLENEMPIRE::SkillSR_FALLENEMPIRE() : WeaponSkillImpl(SR_FALLENEMPIRE) {
}

void SkillSR_FALLENEMPIRE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	// ATK [(Skill Level x 300 + 100) x Caster Base Level / 150] %
	base_skillratio += 300 * skill_lv;
	RE_LVL_DMOD(150);
}

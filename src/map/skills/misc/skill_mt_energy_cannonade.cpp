#include "skill_mt_energy_cannonade.hpp"

SkillMT_ENERGY_CANNONADE::SkillMT_ENERGY_CANNONADE() : WeaponSkillImpl(MT_ENERGY_CANNONADE) {
}

void SkillMT_ENERGY_CANNONADE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 + 750 * skill_lv;
	base_skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	RE_LVL_DMOD(100);
}

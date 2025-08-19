#include "skill_iq_third_flame_bomb.hpp"

SkillIQ_THIRD_FLAME_BOMB::SkillIQ_THIRD_FLAME_BOMB() : WeaponSkillImpl(IQ_THIRD_FLAME_BOMB) {
}

void SkillIQ_THIRD_FLAME_BOMB::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 650 * skill_lv + 10 * sstatus->pow;
	base_skillratio += sstatus->max_hp * 20 / 100;
	RE_LVL_DMOD(100);
}

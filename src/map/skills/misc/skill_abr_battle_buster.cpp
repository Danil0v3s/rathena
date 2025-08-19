#include "skill_abr_battle_buster.hpp"

SkillABR_BATTLE_BUSTER::SkillABR_BATTLE_BUSTER() : WeaponSkillImpl(ABR_BATTLE_BUSTER) {
}

void SkillABR_BATTLE_BUSTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 8000;
}

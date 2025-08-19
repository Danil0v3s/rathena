#include "skill_gn_spore_explosion.hpp"

SkillGN_SPORE_EXPLOSION::SkillGN_SPORE_EXPLOSION() : WeaponSkillImpl(GN_SPORE_EXPLOSION) {
}

void SkillGN_SPORE_EXPLOSION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 400 + 200 * skill_lv;
}

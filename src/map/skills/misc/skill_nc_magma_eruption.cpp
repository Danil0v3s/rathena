#include "skill_nc_magma_eruption.hpp"

SkillNC_MAGMA_ERUPTION::SkillNC_MAGMA_ERUPTION() : WeaponSkillImpl(NC_MAGMA_ERUPTION) {
}

void SkillNC_MAGMA_ERUPTION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 350 + 50 * skill_lv; // 'Slam' damage
}

#include "skill_mh_magma_flow.hpp"

SkillMH_MAGMA_FLOW::SkillMH_MAGMA_FLOW() : WeaponSkillImpl(MH_MAGMA_FLOW) {
}

void SkillMH_MAGMA_FLOW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + (100 * skill_lv + 3 * status_get_lv(src)) * status_get_lv(src) / 120;
}

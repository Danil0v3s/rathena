#include "skill_mh_glanzen_spies.hpp"

SkillMH_GLANZEN_SPIES::SkillMH_GLANZEN_SPIES() : WeaponSkillImpl(MH_GLANZEN_SPIES) {
}

void SkillMH_GLANZEN_SPIES::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 300 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->vit; // !TODO: Confirm VIT bonus
}

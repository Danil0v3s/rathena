#include "skill_mh_midnight_frenzy.hpp"

SkillMH_MIDNIGHT_FRENZY::SkillMH_MIDNIGHT_FRENZY() : WeaponSkillImpl(MH_MIDNIGHT_FRENZY) {
}

void SkillMH_MIDNIGHT_FRENZY::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 450 * skill_lv * status_get_lv(src) / 150 + sstatus->str; // !TODO: Confirm STR bonus
}

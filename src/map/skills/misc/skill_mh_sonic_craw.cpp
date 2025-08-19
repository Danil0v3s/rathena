#include "skill_mh_sonic_craw.hpp"

SkillMH_SONIC_CRAW::SkillMH_SONIC_CRAW() : WeaponSkillImpl(MH_SONIC_CRAW) {
}

void SkillMH_SONIC_CRAW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 60 * skill_lv * status_get_lv(src) / 150;
}

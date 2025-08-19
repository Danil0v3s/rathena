#include "skill_lg_pinpointattack.hpp"

SkillLG_PINPOINTATTACK::SkillLG_PINPOINTATTACK() : WeaponSkillImpl(LG_PINPOINTATTACK) {
}

void SkillLG_PINPOINTATTACK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 100 * skill_lv + 5 * status_get_agi(src);
	RE_LVL_DMOD(120);
}

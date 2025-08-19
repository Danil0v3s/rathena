#include "skill_sr_gentletouch_quiet.hpp"

SkillSR_GENTLETOUCH_QUIET::SkillSR_GENTLETOUCH_QUIET() : WeaponSkillImpl(SR_GENTLETOUCH_QUIET) {
}

void SkillSR_GENTLETOUCH_QUIET::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 100 * skill_lv + sstatus->dex;
	RE_LVL_DMOD(100);
}

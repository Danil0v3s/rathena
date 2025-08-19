#include "skill_mt_powerful_swing.hpp"

SkillMT_POWERFUL_SWING::SkillMT_POWERFUL_SWING() : WeaponSkillImpl(MT_POWERFUL_SWING) {
}

void SkillMT_POWERFUL_SWING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 300 + 850 * skill_lv;
	base_skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	if (sc && sc->getSCE(SC_AXE_STOMP))
		base_skillratio += 100 + 100 * skill_lv;
	RE_LVL_DMOD(100);
}

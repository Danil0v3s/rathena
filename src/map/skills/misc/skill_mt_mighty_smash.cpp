#include "skill_mt_mighty_smash.hpp"

SkillMT_MIGHTY_SMASH::SkillMT_MIGHTY_SMASH() : WeaponSkillImpl(MT_MIGHTY_SMASH) {
}

void SkillMT_MIGHTY_SMASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 80 + 240 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	if (sc && sc->getSCE(SC_AXE_STOMP)) {
		base_skillratio += 20;
		base_skillratio += 5 * sstatus->pow;
	}
	RE_LVL_DMOD(100);
}

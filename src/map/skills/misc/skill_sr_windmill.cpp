#include "skill_sr_windmill.hpp"

SkillSR_WINDMILL::SkillSR_WINDMILL() : WeaponSkillImpl(SR_WINDMILL) {
}

void SkillSR_WINDMILL::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	// ATK [(Caster Base Level + Caster DEX) x Caster Base Level / 100] %
	base_skillratio += -100 + status_get_lv(src) + sstatus->dex;
	RE_LVL_DMOD(100);
}

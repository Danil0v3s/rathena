#include "skill_mt_axe_stomp.hpp"

SkillMT_AXE_STOMP::SkillMT_AXE_STOMP() : WeaponSkillImpl(MT_AXE_STOMP) {
}

void SkillMT_AXE_STOMP::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 450 + 1150 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

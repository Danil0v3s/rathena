#include "skill_dk_servant_w_phantom.hpp"

SkillDK_SERVANT_W_PHANTOM::SkillDK_SERVANT_W_PHANTOM() : WeaponSkillImpl(DK_SERVANT_W_PHANTOM) {
}

void SkillDK_SERVANT_W_PHANTOM::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 + 300 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

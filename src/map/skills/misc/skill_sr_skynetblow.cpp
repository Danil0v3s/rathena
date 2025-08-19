#include "skill_sr_skynetblow.hpp"

SkillSR_SKYNETBLOW::SkillSR_SKYNETBLOW() : WeaponSkillImpl(SR_SKYNETBLOW) {
}

void SkillSR_SKYNETBLOW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	//ATK [{(Skill Level x 200) + (Caster AGI)} x Caster Base Level / 100] %
	base_skillratio += -100 + 200 * skill_lv + sstatus->agi / 6; // !TODO: Confirm AGI bonus
	RE_LVL_DMOD(100);
}

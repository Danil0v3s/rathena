#include "skill_hn_shield_chain_rush.hpp"

SkillHN_SHIELD_CHAIN_RUSH::SkillHN_SHIELD_CHAIN_RUSH() : WeaponSkillImpl(HN_SHIELD_CHAIN_RUSH) {
}

void SkillHN_SHIELD_CHAIN_RUSH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 850 + 1050 * skill_lv;
	base_skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

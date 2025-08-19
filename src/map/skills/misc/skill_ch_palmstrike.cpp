#include "skill_ch_palmstrike.hpp"

SkillCH_PALMSTRIKE::SkillCH_PALMSTRIKE() : WeaponSkillImpl(CH_PALMSTRIKE) {
}

void SkillCH_PALMSTRIKE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 100 + 100 * skill_lv + sstatus->str; // !TODO: How does STR play a role?
	RE_LVL_DMOD(100);
#else
	base_skillratio += 100 + 100 * skill_lv;
#endif
}

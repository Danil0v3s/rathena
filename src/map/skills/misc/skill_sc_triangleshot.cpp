#include "skill_sc_triangleshot.hpp"

SkillSC_TRIANGLESHOT::SkillSC_TRIANGLESHOT() : WeaponSkillImpl(SC_TRIANGLESHOT) {
}

void SkillSC_TRIANGLESHOT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 230 * skill_lv + 3 * sstatus->agi;
	RE_LVL_DMOD(100);
}

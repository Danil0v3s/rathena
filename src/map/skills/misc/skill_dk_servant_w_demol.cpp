#include "skill_dk_servant_w_demol.hpp"

SkillDK_SERVANT_W_DEMOL::SkillDK_SERVANT_W_DEMOL() : WeaponSkillImpl(DK_SERVANT_W_DEMOL) {
}

void SkillDK_SERVANT_W_DEMOL::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 500 * skill_lv;
	RE_LVL_DMOD(100);
}

#include "skill_cd_petitio.hpp"

SkillCD_PETITIO::SkillCD_PETITIO() : WeaponSkillImpl(CD_PETITIO) {
}

void SkillCD_PETITIO::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + (1050 + pc_checkskill(sd,CD_MACE_BOOK_M) * 50) * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

#include "skill_sh_howling_of_chul_ho.hpp"

SkillSH_HOWLING_OF_CHUL_HO::SkillSH_HOWLING_OF_CHUL_HO() : WeaponSkillImpl(SH_HOWLING_OF_CHUL_HO) {
}

void SkillSH_HOWLING_OF_CHUL_HO::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 + 500 * skill_lv;
	base_skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		base_skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 100 * skill_lv;
	if (sd && sd->weapontype1 == W_REVOLVER)
		base_skillratio += 50 + 300 * skill_lv;
	RE_LVL_DMOD(100);
}

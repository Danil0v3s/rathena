#include "skill_nc_powerswing.hpp"

SkillNC_POWERSWING::SkillNC_POWERSWING() : WeaponSkillImpl(NC_POWERSWING) {
}

void SkillNC_POWERSWING::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + ((sstatus->str + sstatus->dex)/ 2) + 300 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_ABR_BATTLE_WARIOR))
		base_skillratio *= 2;
}

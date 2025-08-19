#include "skill_nc_axetornado.hpp"

SkillNC_AXETORNADO::SkillNC_AXETORNADO() : WeaponSkillImpl(NC_AXETORNADO) {
}

void SkillNC_AXETORNADO::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 + 180 * skill_lv + sstatus->vit * 2;
	if (sc && sc->getSCE(SC_AXE_STOMP))
		base_skillratio += 380;
	RE_LVL_DMOD(100);
}

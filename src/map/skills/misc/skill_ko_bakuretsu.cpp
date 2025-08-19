#include "skill_ko_bakuretsu.hpp"

SkillKO_BAKURETSU::SkillKO_BAKURETSU() : WeaponSkillImpl(KO_BAKURETSU) {
}

void SkillKO_BAKURETSU::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + (sd ? pc_checkskill(sd,NJ_TOBIDOUGU) : 1) * (50 + sstatus->dex / 4) * skill_lv * 4 / 10;
	RE_LVL_DMOD(120);
	base_skillratio += 10 * (sd ? sd->status.job_level : 1);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		base_skillratio += base_skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
}

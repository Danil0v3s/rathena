#include "skill_sc_fatalmenace.hpp"

SkillSC_FATALMENACE::SkillSC_FATALMENACE() : WeaponSkillImpl(SC_FATALMENACE) {
}

void SkillSC_FATALMENACE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 120 * skill_lv + sstatus->agi; // !TODO: What's the AGI bonus?

	if( sc != nullptr && sc->getSCE( SC_ABYSS_DAGGER ) ){
		base_skillratio += 30 * skill_lv;
	}

	RE_LVL_DMOD(100);
}

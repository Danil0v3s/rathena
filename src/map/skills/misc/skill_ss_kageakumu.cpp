#include "skill_ss_kageakumu.hpp"

SkillSS_KAGEAKUMU::SkillSS_KAGEAKUMU() : WeaponSkillImpl(SS_KAGEAKUMU) {
}

void SkillSS_KAGEAKUMU::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 18000 + 5 * sstatus->pow;

	if( tsc != nullptr && tsc->getSCE( SC_NIGHTMARE ) != nullptr ){
		base_skillratio += base_skillratio / 2;
	}

	RE_LVL_DMOD(100);
}

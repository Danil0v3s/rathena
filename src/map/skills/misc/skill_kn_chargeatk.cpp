#include "skill_kn_chargeatk.hpp"

SkillKN_CHARGEATK::SkillKN_CHARGEATK() : WeaponSkillImpl(KN_CHARGEATK) {
}

void SkillKN_CHARGEATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	base_skillratio += 600;
#else
	// +100% every 3 cells of distance but hard-limited to 500%
	int32 k = (wd->miscflag-1)/3;
	if (k < 0)
		k = 0;
	else if (k > 4)
		k = 4;
	base_skillratio += 100 * k;
#endif
}

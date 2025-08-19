#include "skill_abc_abyss_dagger.hpp"

SkillABC_ABYSS_DAGGER::SkillABC_ABYSS_DAGGER() : WeaponSkillImpl(ABC_ABYSS_DAGGER) {
}

void SkillABC_ABYSS_DAGGER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 350 + 1400 * skill_lv;
	base_skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	RE_LVL_DMOD(100);
}

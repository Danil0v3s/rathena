#include "skill_nc_boostknuckle.hpp"

SkillNC_BOOSTKNUCKLE::SkillNC_BOOSTKNUCKLE() : WeaponSkillImpl(NC_BOOSTKNUCKLE) {
}

void SkillNC_BOOSTKNUCKLE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 260 * skill_lv + sstatus->dex; // !TODO: What's the DEX bonus?
	RE_LVL_DMOD(100);
}

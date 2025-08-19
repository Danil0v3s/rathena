#include "skill_mt_triple_laser.hpp"

SkillMT_TRIPLE_LASER::SkillMT_TRIPLE_LASER() : WeaponSkillImpl(MT_TRIPLE_LASER) {
}

void SkillMT_TRIPLE_LASER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 650 + 1150 * skill_lv;
	base_skillratio += 12 * sstatus->pow;
	RE_LVL_DMOD(100);
}

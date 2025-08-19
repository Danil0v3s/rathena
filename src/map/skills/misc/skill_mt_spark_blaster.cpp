#include "skill_mt_spark_blaster.hpp"

SkillMT_SPARK_BLASTER::SkillMT_SPARK_BLASTER() : WeaponSkillImpl(MT_SPARK_BLASTER) {
}

void SkillMT_SPARK_BLASTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 600 + 1400 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

#include "skill_ske_noon_blast.hpp"

SkillSKE_NOON_BLAST::SkillSKE_NOON_BLAST() : WeaponSkillImpl(SKE_NOON_BLAST) {
}

void SkillSKE_NOON_BLAST::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1500 + 1250 * skill_lv;
	base_skillratio += pc_checkskill( sd, SKE_SKY_MASTERY ) * 5 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

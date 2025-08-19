#include "skill_ss_kagegari.hpp"

SkillSS_KAGEGARI::SkillSS_KAGEGARI() : WeaponSkillImpl(SS_KAGEGARI) {
}

void SkillSS_KAGEGARI::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 500 + 400 * skill_lv;
	base_skillratio += pc_checkskill( sd, SS_KAGEGISSEN ) * 5 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

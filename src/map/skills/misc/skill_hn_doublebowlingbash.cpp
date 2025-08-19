#include "skill_hn_doublebowlingbash.hpp"

SkillHN_DOUBLEBOWLINGBASH::SkillHN_DOUBLEBOWLINGBASH() : WeaponSkillImpl(HN_DOUBLEBOWLINGBASH) {
}

void SkillHN_DOUBLEBOWLINGBASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 + 400 * skill_lv;
	base_skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

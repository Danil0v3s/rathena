#include "skill_ig_overslash.hpp"

SkillIG_OVERSLASH::SkillIG_OVERSLASH() : WeaponSkillImpl(IG_OVERSLASH) {
}

void SkillIG_OVERSLASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 220 * skill_lv;
	base_skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 50 * skill_lv;
	base_skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
	if ((i = pc_checkskill_imperial_guard(sd, 3)) > 0)
		base_skillratio += base_skillratio * i / 100;
}

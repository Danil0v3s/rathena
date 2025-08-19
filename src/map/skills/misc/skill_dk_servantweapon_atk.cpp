#include "skill_dk_servantweapon_atk.hpp"

SkillDK_SERVANTWEAPON_ATK::SkillDK_SERVANTWEAPON_ATK() : WeaponSkillImpl(DK_SERVANTWEAPON_ATK) {
}

void SkillDK_SERVANTWEAPON_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 600 + 850 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

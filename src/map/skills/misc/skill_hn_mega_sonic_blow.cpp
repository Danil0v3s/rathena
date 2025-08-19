#include "skill_hn_mega_sonic_blow.hpp"

SkillHN_MEGA_SONIC_BLOW::SkillHN_MEGA_SONIC_BLOW() : WeaponSkillImpl(HN_MEGA_SONIC_BLOW) {
}

void SkillHN_MEGA_SONIC_BLOW::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 900 + 750 * skill_lv;
	base_skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 5 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	if (status_get_hp(target) < status_get_max_hp(target) / 2)
		base_skillratio *= 2;
	RE_LVL_DMOD(100);
}

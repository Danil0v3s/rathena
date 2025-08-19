#include "skill_hn_spiral_pierce_max.hpp"

SkillHN_SPIRAL_PIERCE_MAX::SkillHN_SPIRAL_PIERCE_MAX() : WeaponSkillImpl(HN_SPIRAL_PIERCE_MAX) {
}

void SkillHN_SPIRAL_PIERCE_MAX::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 1000 + 1500 * skill_lv;
	base_skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
	switch (status_get_size(target)){
		case SZ_SMALL:
			base_skillratio = base_skillratio * 150 / 100;
			break;
		case SZ_MEDIUM:
			base_skillratio = base_skillratio * 130 / 100;
			break;
		case SZ_BIG:
			base_skillratio = base_skillratio * 120 / 100;
			break;
	}
	RE_LVL_DMOD(100);
}

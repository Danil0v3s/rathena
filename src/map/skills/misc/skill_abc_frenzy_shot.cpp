#include "skill_abc_frenzy_shot.hpp"

SkillABC_FRENZY_SHOT::SkillABC_FRENZY_SHOT() : WeaponSkillImpl(ABC_FRENZY_SHOT) {
}

void SkillABC_FRENZY_SHOT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 + 800 * skill_lv;
	base_skillratio += 15 * sstatus->con;
	RE_LVL_DMOD(100);
}

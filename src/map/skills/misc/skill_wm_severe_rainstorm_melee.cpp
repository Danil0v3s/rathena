#include "skill_wm_severe_rainstorm_melee.hpp"
#include "../../status.hpp"

SkillWM_SEVERE_RAINSTORM_MELEE::SkillWM_SEVERE_RAINSTORM_MELEE() : WeaponSkillImpl(WM_SEVERE_RAINSTORM_MELEE) {
}

void SkillWM_SEVERE_RAINSTORM_MELEE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	//ATK [{(Caster DEX / 300 + AGI / 200)} x Caster Base Level / 100] %
	base_skillratio += -100 + 100 * skill_lv + (status_get_dex(src) / 300 + status_get_agi(src) / 200);
	if (wd->miscflag&4) // Whip/Instrument equipped
		base_skillratio += 20 * skill_lv;
}

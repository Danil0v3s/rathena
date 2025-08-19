#include "skill_wm_great_echo.hpp"
#include "../../pc.hpp"

SkillWM_GREAT_ECHO::SkillWM_GREAT_ECHO() : WeaponSkillImpl(WM_GREAT_ECHO) {
}

void SkillWM_GREAT_ECHO::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 250 + 500 * skill_lv;
	if (src->type == BL_PC) {
		base_skillratio += pc_checkskill((map_session_data*)src, WM_LESSON) * 50; // !TODO: Confirm bonus
		if (skill_check_pc_partner((map_session_data*)src, WM_GREAT_ECHO, &skill_lv, AREA_SIZE, 0) > 0)
			base_skillratio *= 2;
	}
}

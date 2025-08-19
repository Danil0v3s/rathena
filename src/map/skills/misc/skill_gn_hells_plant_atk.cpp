#include "skill_gn_hells_plant_atk.hpp"
#include "../../pc.hpp"

SkillGN_HELLS_PLANT_ATK::SkillGN_HELLS_PLANT_ATK() : WeaponSkillImpl(GN_HELLS_PLANT_ATK) {
}

void SkillGN_HELLS_PLANT_ATK::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 100 * skill_lv + sstatus->int_ * (src->type == BL_PC ? pc_checkskill((map_session_data*)src, AM_CANNIBALIZE) : 5); // !TODO: Confirm INT and Cannibalize bonus
}

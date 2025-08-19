#include "skill_su_svg_spirit.hpp"

SkillSU_SVG_SPIRIT::SkillSU_SVG_SPIRIT() : WeaponSkillImpl(SU_SVG_SPIRIT) {
}

void SkillSU_SVG_SPIRIT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 150 + 150 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		base_skillratio += base_skillratio * status_get_hp(src) / status_get_max_hp(src);
}

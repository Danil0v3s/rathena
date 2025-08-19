#include "skill_ms_bowlingbash.hpp"

SkillMS_BOWLINGBASH::SkillMS_BOWLINGBASH() : WeaponSkillImpl(MS_BOWLINGBASH) {
}

void SkillMS_BOWLINGBASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 40 * skill_lv;
}

#include "skill_kn_bowlingbash.hpp"

SkillKN_BOWLINGBASH::SkillKN_BOWLINGBASH() : WeaponSkillImpl(KN_BOWLINGBASH) {
}

void SkillKN_BOWLINGBASH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 40 * skill_lv;
}

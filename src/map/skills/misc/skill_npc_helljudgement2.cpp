#include "skill_npc_helljudgement2.hpp"

SkillNPC_HELLJUDGEMENT2::SkillNPC_HELLJUDGEMENT2() : WeaponSkillImpl(NPC_HELLJUDGEMENT2) {
}

void SkillNPC_HELLJUDGEMENT2::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
}

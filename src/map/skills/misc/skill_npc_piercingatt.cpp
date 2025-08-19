#include "skill_npc_piercingatt.hpp"

SkillNPC_PIERCINGATT::SkillNPC_PIERCINGATT() : WeaponSkillImpl(NPC_PIERCINGATT) {
}

void SkillNPC_PIERCINGATT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -25; //75% base damage
}

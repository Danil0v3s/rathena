#include "skill_npc_vampire_gift.hpp"

SkillNPC_VAMPIRE_GIFT::SkillNPC_VAMPIRE_GIFT() : WeaponSkillImpl(NPC_VAMPIRE_GIFT) {
}

void SkillNPC_VAMPIRE_GIFT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += ((skill_lv - 1) % 5 + 1) * 100;
}

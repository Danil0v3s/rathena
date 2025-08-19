#include "skill_rk_stormblast.hpp"

SkillRK_STORMBLAST::SkillRK_STORMBLAST() : WeaponSkillImpl(RK_STORMBLAST) {
}

void SkillRK_STORMBLAST::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + (((sd) ? pc_checkskill(sd,RK_RUNEMASTERY) : 0) + sstatus->str / 6) * 100; // ATK = [{Rune Mastery Skill Level + (Caster's STR / 6)} x 100] %
	RE_LVL_DMOD(100);
}

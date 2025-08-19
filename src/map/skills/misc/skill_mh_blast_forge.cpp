#include "skill_mh_blast_forge.hpp"

SkillMH_BLAST_FORGE::SkillMH_BLAST_FORGE() : WeaponSkillImpl(MH_BLAST_FORGE) {
}

void SkillMH_BLAST_FORGE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 70 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
}

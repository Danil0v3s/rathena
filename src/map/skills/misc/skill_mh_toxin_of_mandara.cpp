#include "skill_mh_toxin_of_mandara.hpp"

SkillMH_TOXIN_OF_MANDARA::SkillMH_TOXIN_OF_MANDARA() : WeaponSkillImpl(MH_TOXIN_OF_MANDARA) {
}

void SkillMH_TOXIN_OF_MANDARA::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 400 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
}

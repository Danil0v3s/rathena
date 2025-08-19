#include "skill_ko_setsudan.hpp"

SkillKO_SETSUDAN::SkillKO_SETSUDAN() : WeaponSkillImpl(KO_SETSUDAN) {
}

void SkillKO_SETSUDAN::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 100 * (skill_lv - 1);
	RE_LVL_DMOD(100);
	if (tsc) {
		struct status_change_entry *sce;

		if ((sce = tsc->getSCE(SC_SPIRIT)) || (sce = tsc->getSCE(SC_SOULGOLEM)) || (sce = tsc->getSCE(SC_SOULSHADOW)) || (sce = tsc->getSCE(SC_SOULFALCON)) || (sce = tsc->getSCE(SC_SOULFAIRY))) // Bonus damage added when target is soul linked.
			base_skillratio += 200 * sce->val1;
	}
}

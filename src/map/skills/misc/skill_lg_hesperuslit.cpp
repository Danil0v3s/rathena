#include "skill_lg_hesperuslit.hpp"

SkillLG_HESPERUSLIT::SkillLG_HESPERUSLIT() : WeaponSkillImpl(LG_HESPERUSLIT) {
}

void SkillLG_HESPERUSLIT::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	if (sc && sc->getSCE(SC_INSPIRATION))
		base_skillratio += -100 + 450 * skill_lv;
	else
		base_skillratio += -100 + 300 * skill_lv;
	base_skillratio += sstatus->vit / 6; // !TODO: What's the VIT bonus?
	RE_LVL_DMOD(100);
}

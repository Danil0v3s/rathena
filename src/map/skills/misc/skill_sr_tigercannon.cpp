#include "skill_sr_tigercannon.hpp"

SkillSR_TIGERCANNON::SkillSR_TIGERCANNON() : WeaponSkillImpl(SR_TIGERCANNON) {
}

void SkillSR_TIGERCANNON::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	{
		uint32 hp = sstatus->max_hp * (10 + (skill_lv * 2)) / 100,
					 sp = sstatus->max_sp * (5 + skill_lv) / 100;

		if (wd->miscflag&8)
			// Base_Damage = [((Caster consumed HP + SP) / 2) x Caster Base Level / 100] %
			base_skillratio += -100 + (hp + sp) / 2;
		else
			// Base_Damage = [((Caster consumed HP + SP) / 4) x Caster Base Level / 100] %
			base_skillratio += -100 + (hp + sp) / 4;
		RE_LVL_DMOD(100);
	}
	if (sc->getSCE(SC_GT_REVITALIZE))
		base_skillratio += base_skillratio * 30 / 100;
}

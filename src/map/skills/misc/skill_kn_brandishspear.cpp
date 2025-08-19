#include "skill_kn_brandishspear.hpp"

SkillKN_BRANDISHSPEAR::SkillKN_BRANDISHSPEAR() : WeaponSkillImpl(KN_BRANDISHSPEAR) {
}

void SkillKN_BRANDISHSPEAR::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
#ifdef RENEWAL
	status_data* sstatus = status_get_status_data(src);
	base_skillratio += -100 + 400 + 100 * skill_lv + sstatus->str * 3;
#endif
}

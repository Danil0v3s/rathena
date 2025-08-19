#include "skill_iq_exposion_blaster.hpp"

SkillIQ_EXPOSION_BLASTER::SkillIQ_EXPOSION_BLASTER() : WeaponSkillImpl(IQ_EXPOSION_BLASTER) {
}

void SkillIQ_EXPOSION_BLASTER::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 450 + 2600 * skill_lv;
	base_skillratio += 10 * sstatus->pow;

	if( tsc != nullptr && tsc->getSCE( SC_HOLY_OIL ) ){
		base_skillratio += 950 * skill_lv;
	}
}

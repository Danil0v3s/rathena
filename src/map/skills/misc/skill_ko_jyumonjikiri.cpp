#include "skill_ko_jyumonjikiri.hpp"
#include "../../pc.hpp"
#include "../../status.hpp"

SkillKO_JYUMONJIKIRI::SkillKO_JYUMONJIKIRI() : WeaponSkillImpl(KO_JYUMONJIKIRI) {
}

void SkillKO_JYUMONJIKIRI::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += -100 + 200 * skill_lv;
	if(tsc && tsc->getSCE(SC_JYUMONJIKIRI))
		base_skillratio += skill_lv * status_get_lv(src);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		base_skillratio += base_skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
}

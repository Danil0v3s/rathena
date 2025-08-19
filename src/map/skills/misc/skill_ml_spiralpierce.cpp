#include "skill_ml_spiralpierce.hpp"

SkillML_SPIRALPIERCE::SkillML_SPIRALPIERCE() : WeaponSkillImpl(ML_SPIRALPIERCE) {
}

#ifdef RENEWAL
void SkillML_SPIRALPIERCE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50 + 50 * skill_lv;
}
#endif

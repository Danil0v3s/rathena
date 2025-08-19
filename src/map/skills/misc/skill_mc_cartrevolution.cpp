#include "skill_mc_cartrevolution.hpp"

SkillMC_CARTREVOLUTION::SkillMC_CARTREVOLUTION() : WeaponSkillImpl(MC_CARTREVOLUTION) {
}

void SkillMC_CARTREVOLUTION::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	base_skillratio += 50;
	// Note: sd (player data) check would need to be implemented separately
	// if(sd && sd->cart_weight)
	//     base_skillratio += 100 * sd->cart_weight / sd->cart_weight_max; // +1% every 1% weight
	// else if (!sd)
	//     base_skillratio += 100; //Max damage for non players.
}

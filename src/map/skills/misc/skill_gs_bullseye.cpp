#include "skill_gs_bullseye.hpp"

SkillGS_BULLSEYE::SkillGS_BULLSEYE() : WeaponSkillImpl(GS_BULLSEYE) {
}

void SkillGS_BULLSEYE::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	//Only works well against brute/demihumans non bosses.
	if((tstatus->race == RC_BRUTE || tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_PLAYER_HUMAN || tstatus->race == RC_PLAYER_DORAM) && !status_has_mode(tstatus,MD_STATUSIMMUNE))
		base_skillratio += 400;
}

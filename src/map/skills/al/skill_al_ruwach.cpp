#include "skill_al_ruwach.hpp"

#include "../../agent/battle.hpp"
#include "../../agent/skill.hpp"
#include "../../status.hpp"
#include "../../clif.hpp"

SkillAL_RUWACH::SkillAL_RUWACH() : SkillImpl(AL_RUWACH) {
}

void SkillAL_RUWACH::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// No additional effects for this skill
}

void SkillAL_RUWACH::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	// No damage for this skill
}

void SkillAL_RUWACH::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	// No hit rate modification for this skill
}

int32 SkillAL_RUWACH::castendNoDamageId(struct block_list *src, struct block_list *bl, uint16 skill_lv, t_tick tick, int32 flag) const {
	struct status_change *tsc = nullptr;
	enum sc_type type = skill_get_sc(skill_id);

	if (bl->type == BL_PC)
		tsc = &((TBL_PC*)bl)->sc;
	else if (bl->type == BL_MOB)
		tsc = &((TBL_MOB*)bl)->sc;

	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start2(src, bl, type, 100, skill_lv, skill_id, skill_get_time(skill_id, skill_lv)));
	
	return 0;
}
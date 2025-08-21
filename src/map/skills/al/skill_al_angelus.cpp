#include "skill_al_angelus.hpp"

#include "../../agent/battle.hpp"
#include "../../agent/skill.hpp"
#include "../../status.hpp"
#include "../../clif.hpp"
#include "../../party.hpp"

SkillAL_ANGELUS::SkillAL_ANGELUS() : SkillImpl(AL_ANGELUS) {
}

void SkillAL_ANGELUS::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// No additional effects for this skill
}

void SkillAL_ANGELUS::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	// No damage for this skill
}

void SkillAL_ANGELUS::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	// No hit rate modification for this skill
}

int32 SkillAL_ANGELUS::castendNoDamageId(struct block_list *src, struct block_list *bl, uint16 skill_lv, t_tick tick, int32 flag) const {
	struct map_session_data *sd = nullptr;
	struct status_change *tsc = nullptr;
	enum sc_type type = skill_get_sc(skill_id);

	if (src->type == BL_PC)
		sd = (TBL_PC*)src;

	if (bl->type == BL_PC)
		tsc = &((TBL_PC*)bl)->sc;
	else if (bl->type == BL_MOB)
		tsc = &((TBL_MOB*)bl)->sc;

#ifdef RENEWAL
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		// Animations don't play when outside visible range
		if (check_distance_bl(src, bl, AREA_SIZE))
			clif_skill_nodamage(bl, *bl, skill_id, skill_lv);

		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	} else {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
#else
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		// Animations don't play when outside visible range
		if (check_distance_bl(src, bl, AREA_SIZE))
			clif_skill_nodamage(bl, *bl, skill_id, skill_lv);

		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	} else {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
#endif

	return 0;
}
#include "skill_al_incagi.hpp"

#include "../../agent/battle.hpp"
#include "../../agent/skill.hpp"
#include "../../status.hpp"
#include "../../clif.hpp"

SkillAL_INCAGI::SkillAL_INCAGI() : SkillImpl(AL_INCAGI) {
}

void SkillAL_INCAGI::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// No additional effects for this skill
}

void SkillAL_INCAGI::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio) const {
	// No damage for this skill
}

void SkillAL_INCAGI::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	// No hit rate modification for this skill
}

int32 SkillAL_INCAGI::castendNoDamageId(struct block_list *src, struct block_list *bl, uint16 skill_lv, t_tick tick, int32 flag) const {
	struct map_session_data *sd = nullptr, *dstsd = nullptr;
	struct status_change *tsc = nullptr;
	enum sc_type type = skill_get_sc(skill_id);

	if (src->type == BL_PC)
		sd = (TBL_PC*)src;

	if (bl->type == BL_PC) {
		dstsd = (TBL_PC*)bl;
		tsc = &((TBL_PC*)bl)->sc;
	} else if (bl->type == BL_MOB)
		tsc = &((TBL_MOB*)bl)->sc;

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (dstsd != nullptr && tsc && tsc->getSCE(SC_CHANGEUNDEAD))
	{
		if (status_get_hp(bl) > 1)
			// This would normally call skill_attack, but we're just showing the structure
			// skill_attack(BF_MISC, src, src, bl, skill_id, skill_lv, tick, flag);
			return 0;
	}
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	
	return 0;
}
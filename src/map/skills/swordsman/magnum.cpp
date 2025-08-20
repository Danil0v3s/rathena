#include "magnum.hpp"
#include "../../clif.hpp"
#include "../../status.hpp"
#include "../../unit.hpp"
#include "../../skill.hpp"

SkillMagnumBreak::SkillMagnumBreak() : WeaponSkillImpl(SM_MAGNUM)
{
}

void SkillMagnumBreak::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio) const
{
	if (wd->miscflag == 1)
		base_skillratio += 20 * skill_lv; // Inner 3x3 circle takes 100%+20%*level damage [Playtester]
	else
		base_skillratio += 10 * skill_lv; // Outer 5x5 circle takes 100%+10%*level damage [Playtester]
}

void SkillMagnumBreak::modifyHitRate(int16 &hit_rate, const block_list *src, const block_list *target, uint16 skill_lv) const
{
	hit_rate += hit_rate * 10 * skill_lv / 100;
}

void SkillMagnumBreak::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 flag) const
{
	if (flag & 1)
	{
		// For players, damage depends on distance, so add it to flag if it is > 1
		// Cannot hit hidden targets
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag | SD_ANIMATION);
	}
}

void SkillMagnumBreak::castendNoDamageId(struct block_list *src, struct block_list *bl, uint16 skill_id, uint16 skill_lv, t_tick tick, int32 flag) const
{
	// Passive Magnum, should had been casted on yourself.
	extern int32 skill_area_temp[8];
	extern int32 skill_area_sub(struct block_list * bl, va_list ap);
	skill_area_temp[1] = 0;
	map_foreachinshootrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_SKILL | BL_CHAR,
							src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
	// Initiate 20% of your damage becomes fire element.
#ifdef RENEWAL
	sc_start4(src, src, SC_SUB_WEAPONPROPERTY, 100, ELE_FIRE, 20, skill_id, 0, skill_get_time2(skill_id, skill_lv));
#else
	sc_start4(src, src, SC_WATK_ELEMENT, 100, ELE_FIRE, 20, 0, 0, skill_get_time2(skill_id, skill_lv));
#endif
}

switch (skill_id)
{
case 0:
{ // Normal attacks (no skill used)
	if (attack_type & BF_SKILL)
		break; // If a normal attack is a skill, it's splash damage. [Inkfish]
	if (sd)
	{
		int32 skill;

		// Automatic trigger of Blitz Beat
		if (pc_isfalcon(sd) && sd->status.weapon == W_BOW && (skill = pc_checkskill(sd, HT_BLITZBEAT)) > 0 && rnd() % 1000 <= sstatus->luk * 10 / 3 + 1)
		{
			int32 rate;

			if ((sd->class_ & MAPID_THIRDMASK) == MAPID_RANGER)
				rate = 5;
			else
				rate = (sd->status.job_level + 9) / 10;

			skill_castend_damage_id(src, bl, HT_BLITZBEAT, (skill < rate) ? skill : rate, tick, SD_LEVEL);
		}
		// Automatic trigger of Warg Strike
		if (pc_iswug(sd) && (skill = pc_checkskill(sd, RA_WUGSTRIKE)) > 0)
		{
			int32 rate = sstatus->luk * 10 / 3 + 1;

			if (pc_isfalcon(sd))
				rate = rate / 3;

			if (rnd() % 1000 <= rate)
				skill_castend_damage_id(src, bl, RA_WUGSTRIKE, skill, tick, 0);
		}
		// Automatic trigger of Hawk Rush
		if (pc_isfalcon(sd) && sd->status.weapon == W_BOW && (skill = pc_checkskill(sd, WH_HAWKRUSH)) > 0)
		{
			int32 rate = sstatus->con * 10 / 3 + 1;

			rate += rate * (20 * pc_checkskill(sd, WH_NATUREFRIENDLY)) / 100;

			if (rnd() % 1000 <= rate)
				skill_castend_damage_id(src, bl, WH_HAWKRUSH, skill, tick, 0);
		}
		// Gank
		if (dstmd && sd->status.weapon != W_BOW &&
			(skill = pc_checkskill(sd, RG_SNATCHER)) > 0 &&
			(skill * 15 + 55) + pc_checkskill(sd, TF_STEAL) * 10 > rnd() % 1000)
		{
			if (pc_steal_item(sd, bl, pc_checkskill(sd, TF_STEAL)))
				clif_skill_nodamage(src, *bl, TF_STEAL, skill);
			else
				clif_skill_fail(*sd, RG_SNATCHER);
		}
	}

	if (sc)
	{
		struct status_change_entry *sce;
		// Enchant Poison gives a chance to poison attacked enemies
		if ((sce = sc->getSCE(SC_ENCPOISON))) // Don't use sc_start since chance comes in 1/10000 rate.
			status_change_start(src, bl, SC_POISON, sce->val2, sce->val1, src->id, 0, 0,
								skill_get_time2(AS_ENCHANTPOISON, sce->val1), SCSTART_NONE);
		// Enchant Deadly Poison gives a chance to deadly poison attacked enemies
		if ((sce = sc->getSCE(SC_EDP)))
			sc_start4(src, bl, SC_DPOISON, sce->val2, sce->val1, src->id, 0, 0,
					  skill_get_time2(ASC_EDP, sce->val1));
		if ((sce = sc->getSCE(SC_LUXANIMA)) && rnd() % 100 < sce->val2)
			skill_castend_nodamage_id(src, bl, RK_STORMBLAST, 1, tick, 0);
	}
}
break;

case MER_CRASH:
	sc_start(src, bl, SC_STUN, (6 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case AS_VENOMKNIFE:
case AS_SPLASHER:
	sc_start2(src, bl, SC_POISON, 100, skill_lv, src->id, skill_get_time2(skill_id, skill_lv));
	break;

case TF_POISON:
	if (!sc_start2(src, bl, SC_POISON, (4 * skill_lv + 10), skill_lv, src->id, skill_get_time2(skill_id, skill_lv)) && sd)
		clif_skill_fail(*sd, skill_id);
	break;

case AS_SONICBLOW:
case HN_MEGA_SONIC_BLOW:
	if (!map_flag_gvg2(bl->m) && !map_getmapflag(bl->m, MF_BATTLEGROUND) && sc && sc->getSCE(SC_SPIRIT) && sc->getSCE(SC_SPIRIT)->val2 == SL_ASSASIN)
		sc_start(src, bl, SC_STUN, (4 * skill_lv + 20), skill_lv, skill_get_time2(skill_id, skill_lv)); // Link gives double stun chance outside GVG/BG
	else
		sc_start(src, bl, SC_STUN, (2 * skill_lv + 10), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case AS_GRIMTOOTH:
	if (dstmd && !status_has_mode(tstatus, MD_STATUSIMMUNE))
		sc_start(src, bl, SC_QUAGMIRE, 100, 0, skill_get_time2(skill_id, skill_lv));
	break;

case WZ_FIREPILLAR:
	unit_set_walkdelay(bl, tick, skill_get_time2(skill_id, skill_lv), 1);
	break;

case MG_FROSTDIVER:
	if (!sc_start(src, bl, SC_FREEZE, min(skill_lv * 3 + 35, skill_lv + 60), skill_lv, skill_get_time2(skill_id, skill_lv)) && sd)
		clif_skill_fail(*sd, skill_id);
	break;

case WZ_FROSTNOVA:
	sc_start(src, bl, SC_FREEZE, (sd != nullptr) ? skill_lv * 5 + 33 : skill_lv * 3 + 35, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case WZ_STORMGUST:
	// Storm Gust counter was dropped in renewal
#ifdef RENEWAL
	sc_start(src, bl, SC_FREEZE, 65 - (5 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
#else
	// On third hit, there is a 150% to freeze the target
	if (tsc->sg_counter >= 3 &&
		sc_start(src, bl, SC_FREEZE, 150, skill_lv, skill_get_time2(skill_id, skill_lv)))
		tsc->sg_counter = 0;
	// Being it only resets on success it'd keep stacking and eventually overflowing on mvps, so we reset at a high value
	else if (tsc->sg_counter > 250)
		tsc->sg_counter = 0;
#endif
	break;

case NPC_STORMGUST2:
	if (skill_lv == 1)
		sc_start(src, bl, SC_FREEZE, 10, skill_lv, skill_get_time2(skill_id, skill_lv));
	else if (skill_lv == 2)
		sc_start(src, bl, SC_FREEZE, 7, skill_lv, skill_get_time2(skill_id, skill_lv));
	else
		sc_start(src, bl, SC_FREEZE, 3, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case WZ_METEOR:
case HN_METEOR_STORM_BUSTER:
	sc_start(src, bl, SC_STUN, 3 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case WZ_VERMILION:
#ifdef RENEWAL
	sc_start(src, bl, SC_BLIND, 10 + 5 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
#else
	sc_start(src, bl, SC_BLIND, min(4 * skill_lv, 40), skill_lv, skill_get_time2(skill_id, skill_lv));
#endif
	break;

case WZ_HEAVENDRIVE:
	status_change_end(bl, SC_SV_ROOTTWIST);
	break;

case HT_FREEZINGTRAP:
case MA_FREEZINGTRAP:
	sc_start(src, bl, SC_FREEZE, 100, skill_lv, skill_get_time2(skill_id, skill_lv), sstatus->amotion + 100);
	break;

case HT_FLASHER:
	sc_start(src, bl, SC_BLIND, 100, skill_lv, skill_get_time2(skill_id, skill_lv), 1000);
	break;

case HT_LANDMINE:
case MA_LANDMINE:
	sc_start(src, bl, SC_STUN, 10, skill_lv, skill_get_time2(skill_id, skill_lv), 1000);
	break;

case HT_SHOCKWAVE:
	status_percent_damage(src, bl, 0, -(15 * skill_lv + 5), false);
	break;

case HT_SANDMAN:
case MA_SANDMAN:
	sc_start(src, bl, SC_SLEEP, (10 * skill_lv + 40), skill_lv, skill_get_time2(skill_id, skill_lv), 1000);
	break;

case TF_SPRINKLESAND:
	sc_start(src, bl, SC_BLIND, (sd != nullptr) ? 20 : 15, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case TF_THROWSTONE:
	if (sd != nullptr)
	{
		// Only blind if used by player and stun failed
		if (!sc_start(src, bl, SC_STUN, 3, skill_lv, skill_get_time(skill_id, skill_lv)))
			sc_start(src, bl, SC_BLIND, 3, skill_lv, skill_get_time2(skill_id, skill_lv));
	}
	else
	{
		// 5% stun chance and no blind chance when used by monsters
		sc_start(src, bl, SC_STUN, 5, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	break;

case NPC_DARKCROSS:
case CR_HOLYCROSS:
	sc_start(src, bl, SC_BLIND, 3 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case NPC_GRANDDARKNESS:
	sc_start(src, bl, SC_BLIND, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case CR_GRANDCROSS:
	// Chance to cause blind status vs demon and undead element, but not against players
	if (!dstsd && (battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON))
		sc_start(src, bl, SC_BLIND, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case AM_ACIDTERROR:
	sc_start2(src, bl, SC_BLEEDING, (skill_lv * 3), skill_lv, src->id, skill_get_time2(skill_id, skill_lv));
#ifdef RENEWAL
	if (skill_break_equip(src, bl, EQP_ARMOR, (1000 * skill_lv + 500) - 1000, BCT_ENEMY))
#else
	if (skill_break_equip(src, bl, EQP_ARMOR, 100 * skill_get_time(skill_id, skill_lv), BCT_ENEMY))
#endif
		clif_emotion(*bl, ET_HUK);
	break;

case AM_DEMONSTRATION:
#ifdef RENEWAL
	skill_break_equip(src, bl, EQP_WEAPON, 300 * skill_lv, BCT_ENEMY);
#else
	skill_break_equip(src, bl, EQP_WEAPON, 100 * skill_lv, BCT_ENEMY);
#endif
	break;

case CR_SHIELDCHARGE:
	sc_start(src, bl, SC_STUN, (15 + skill_lv * 5), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

#ifndef RENEWAL
case PA_PRESSURE:
	status_percent_damage(src, bl, 0, 15 + 5 * skill_lv, false);
	[[fallthrough]];
case HW_GRAVITATION:
	// Pressure and Gravitation can trigger physical autospells
	attack_type |= BF_NORMAL;
	attack_type |= BF_WEAPON;
	break;
#endif

case RG_RAID:
	sc_start(src, bl, SC_STUN, (10 + 3 * skill_lv), skill_lv, skill_get_time(skill_id, skill_lv));
	sc_start(src, bl, SC_BLIND, (10 + 3 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
#ifdef RENEWAL
	sc_start(src, bl, SC_RAID, 100, skill_lv, 10000); // Hardcoded to 10 seconds since Duration1 and Duration2 are used
	break;

case RG_BACKSTAP:
	sc_start(src, bl, SC_STUN, (5 + 2 * skill_lv), skill_lv, skill_get_time(skill_id, skill_lv));
#endif
	break;

case BA_FROSTJOKER:
case DC_SCREAM:
{
	int32 rate = 150 + 50 * skill_lv; // Aegis accuracy (1000 = 100%)
	int32 duration = skill_get_time2(skill_id, skill_lv);
	if (skill_id == DC_SCREAM)
		rate += 100; // DC_SCREAM has a 10% higher base chance
	if (battle_check_target(src, bl, BCT_PARTY) > 0)
	{
		// On party members: Chance is divided by 4 and BA_FROSTJOKER duration is fixed to 15000ms
		rate /= 4;
		duration = skill_get_time(skill_id, skill_lv);
	}
	status_change_start(src, bl, skill_get_sc(skill_id), rate * 10, skill_lv, 0, 0, 0, duration, SCSTART_NONE);
}
break;

case BD_LULLABY:
#ifndef RENEWAL
	if (sc != nullptr && sc->getSCE(SC_DANCING) != nullptr)
	{
		block_list *partner = map_id2bl(sc->getSCE(SC_DANCING)->val4);
		if (partner == nullptr)
			break;
		status_data *pstatus = status_get_status_data(*partner);
		if (pstatus == nullptr)
			break;
		status_change_start(src, bl, skill_get_sc(skill_id), (sstatus->int_ + pstatus->int_ + rnd_value(100, 300)) * 10, skill_lv, 0, 0, 0, skill_get_time2(skill_id, skill_lv), SCSTART_NONE);
	}
#else
	// In renewal the chance is simply 100% and uses the original song duration as sleep duration
	sc_start(src, bl, skill_get_sc(skill_id), 100, skill_lv, skill_get_time(skill_id, skill_lv));
#endif
	break;

#ifdef RENEWAL
case DC_UGLYDANCE:
	// !TODO: How does caster's DEX/AGI play a role?
	status_zap(bl, 0, 2 * skill_lv + 10);
	break;
#else
case DC_UGLYDANCE:
{
	int32 rate = 5 + 5 * skill_lv;
	rate += skill_lv * pc_checkskill(sd, DC_DANCINGLESSON);
	status_zap(bl, 0, rate);
}
break;
#endif
case SL_STUN:
	if (tstatus->size == SZ_MEDIUM) // Only stuns mid-sized mobs.
		sc_start(src, bl, SC_STUN, (30 + 10 * skill_lv), skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case NPC_PETRIFYATTACK:
	sc_start2(src, bl, SC_STONEWAIT, (20 * skill_lv), skill_lv, src->id, skill_get_time2(skill_id, skill_lv), skill_get_time(skill_id, skill_lv));
	break;
case NPC_CURSEATTACK:
	sc_start(src, bl, SC_CURSE, (20 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_SLEEPATTACK:
	sc_start(src, bl, SC_SLEEP, (20 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_BLINDATTACK:
	sc_start(src, bl, SC_BLIND, (20 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_POISON:
	sc_start(src, bl, SC_POISON, (20 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_SILENCEATTACK:
	sc_start(src, bl, SC_SILENCE, (20 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_STUNATTACK:
	sc_start(src, bl, SC_STUN, (20 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_BLEEDING:
	sc_start(src, bl, SC_BLEEDING, (20 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_BLEEDING2:
	sc_start(src, bl, SC_BLEEDING, (50 + 10 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_CHANGEUNDEAD:
	sc_start(src, bl, SC_CHANGEUNDEAD, (10 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_ACIDBREATH:
	sc_start(src, bl, SC_POISON, 70, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_ICEBREATH:
	sc_start(src, bl, SC_FREEZE, 70, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_ICEBREATH2:
	sc_start(src, bl, SC_FREEZE, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_MENTALBREAKER:
{ // Based on observations by Tharis, Mental Breaker should do SP damage
	// equal to Matk*skLevel.
	int32 rate = sstatus->matk_min;
	if (rate < sstatus->matk_max)
		rate += rnd() % (sstatus->matk_max - sstatus->matk_min);
	rate *= skill_lv;
	status_zap(bl, 0, rate);
	break;
}
// Equipment breaking monster skills [Celest]
case NPC_ARMORBRAKE:
	skill_break_equip(src, bl, EQP_ARMOR, 150 * skill_lv, BCT_ENEMY);
	break;
case NPC_HELMBRAKE:
	skill_break_equip(src, bl, EQP_HELM, 150 * skill_lv, BCT_ENEMY);
	break;
case NPC_SHIELDBRAKE:
	skill_break_equip(src, bl, EQP_SHIELD, 150 * skill_lv, BCT_ENEMY);
	break;

case CH_TIGERFIST:
{
	t_tick basetime = skill_get_time(skill_id, skill_lv);
	t_tick mintime = 15 * (status_get_lv(src) + 100);

	if (status_bl_has_mode(bl, MD_STATUSIMMUNE))
		basetime /= 5;
	basetime = std::max((basetime * status_get_agi(bl)) / -200 + basetime, mintime);
	sc_start(src, bl, SC_ANKLE, (1 + skill_lv) * 10, 0, basetime);
}
break;

case LK_SPIRALPIERCE:
case ML_SPIRALPIERCE:
case HN_SPIRAL_PIERCE_MAX:
	if (dstsd || (dstmd && !status_bl_has_mode(bl, MD_STATUSIMMUNE))) // Does not work on status immune
		sc_start(src, bl, SC_ANKLE, 100, 0, skill_get_time2(skill_id, skill_lv));
	break;

case ST_REJECTSWORD:
	sc_start(src, bl, SC_AUTOCOUNTER, (skill_lv * 15), skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case PF_FOGWALL:
	if (src != bl && !tsc->getSCE(SC_DELUGE))
		sc_start(src, bl, SC_BLIND, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case LK_HEADCRUSH: // Headcrush has chance of causing Bleeding status, except on demon and undead element
	if (!(battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON))
		sc_start2(src, bl, SC_BLEEDING, 50, skill_lv, src->id, skill_get_time2(skill_id, skill_lv));
	break;

case ASC_METEORASSAULT:
	// Any enemies hit by this skill will receive Stun, Darkness, or external bleeding status ailment with a 5%+5*skill_lv% chance.
	switch (rnd() % 3)
	{
	case 0:
		sc_start(src, bl, SC_BLIND, (5 + skill_lv * 5), skill_lv, skill_get_time2(skill_id, 1));
		break;
	case 1:
		sc_start(src, bl, SC_STUN, (5 + skill_lv * 5), skill_lv, skill_get_time2(skill_id, 2));
		break;
	default:
		sc_start2(src, bl, SC_BLEEDING, (5 + skill_lv * 5), skill_lv, src->id, skill_get_time2(skill_id, 3));
	}
	break;

case HW_NAPALMVULCAN:
case HN_NAPALM_VULCAN_STRIKE:
	sc_start(src, bl, SC_CURSE, 5 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case WS_CARTTERMINATION: // Cart termination
	sc_start(src, bl, SC_STUN, 5 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case CR_ACIDDEMONSTRATION:
case GN_FIRE_EXPANSION_ACID:
	skill_break_equip(src, bl, EQP_WEAPON | EQP_ARMOR, 100 * skill_lv, BCT_ENEMY);
	break;

case TK_DOWNKICK:
	sc_start(src, bl, SC_STUN, 3333, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case TK_JUMPKICK:
	// debuff the following statuses
	if (dstsd && dstsd->class_ != MAPID_SOUL_LINKER && !tsc->getSCE(SC_PRESERVE))
	{
		status_change_end(bl, SC_SPIRIT);
		status_change_end(bl, SC_ADRENALINE2);
		status_change_end(bl, SC_KAITE);
		status_change_end(bl, SC_KAAHI);
		status_change_end(bl, SC_ONEHAND);
		status_change_end(bl, SC_ASPDPOTION2);
		// New soul links confirmed to not dispell with this skill
		// but thats likely a bug since soul links can't stack and
		// soul cutter skill works on them. So ill add this here for now. [Rytech]
		status_change_end(bl, SC_SOULGOLEM);
		status_change_end(bl, SC_SOULSHADOW);
		status_change_end(bl, SC_SOULFALCON);
		status_change_end(bl, SC_SOULFAIRY);
	}
	break;
case TK_TURNKICK:
	// Note: attack_type is passed as BF_WEAPON for the actual target, BF_MISC for the splash-affected mobs.
	if (attack_type & BF_MISC)
	{
		sc_start(src, bl, SC_STUN, 200, skill_lv, skill_get_time(skill_id, skill_lv));
		clif_specialeffect(bl, EF_SPINEDBODY, AREA);
		sc_start(src, bl, SC_NOACTION, 100, 1, skill_get_time2(skill_id, skill_lv));
	}
	break;
case MO_BALKYOUNG:			   // Note: attack_type is passed as BF_WEAPON for the actual target, BF_MISC for the splash-affected mobs.
	if (attack_type & BF_MISC) // 70% base stun chance...
		sc_start(src, bl, SC_STUN, 70, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case GS_BULLSEYE: // 0.1% coma rate.
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_PLAYER_HUMAN || tstatus->race == RC_PLAYER_DORAM)
		status_change_start(src, bl, SC_COMA, 10, skill_lv, 0, src->id, 0, 0, SCSTART_NONE);
	break;
case GS_PIERCINGSHOT:
	sc_start2(src, bl, SC_BLEEDING, (skill_lv * 3), skill_lv, src->id, skill_get_time2(skill_id, skill_lv));
	break;
case NJ_HYOUSYOURAKU:
	sc_start(src, bl, SC_FREEZE, (10 + 10 * skill_lv), skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case GS_FLING:
	sc_start(src, bl, SC_FLING, 100, sd ? sd->spiritball_old : 5, skill_get_time(skill_id, skill_lv));
	break;
case GS_DISARM:
	skill_strip_equip(src, bl, skill_id, skill_lv);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case NPC_EVILLAND:
	sc_start(src, bl, SC_BLIND, 5 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_HELLJUDGEMENT:
	sc_start(src, bl, SC_CURSE, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_HELLJUDGEMENT2:
	switch (rnd() % 6)
	{
	case 0:
		sc_start(src, bl, SC_SLEEP, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
		break;
	case 1:
		sc_start(src, bl, SC_CONFUSION, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
		break;
	case 2:
		sc_start(src, bl, SC_HALLUCINATION, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
		break;
	case 3:
		sc_start(src, bl, SC_STUN, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
		break;
	case 4:
		sc_start(src, bl, SC_FEAR, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
		break;
	default:
		sc_start(src, bl, SC_CURSE, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
		break;
	}
	break;
case NPC_CRITICALWOUND:
case NPC_WIDECRITICALWOUND:
	sc_start(src, bl, SC_CRITICALWOUND, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_FIRESTORM:
	sc_start(src, bl, SC_BURNT, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case RK_DRAGONBREATH:
	sc_start4(src, bl, SC_BURNING, 15, skill_lv, 1000, src->id, 0, skill_get_time(skill_id, skill_lv));
	break;
case RK_DRAGONBREATH_WATER:
	sc_start(src, bl, SC_FREEZING, 15, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case NPC_DRAGONBREATH:
	if (skill_lv > 5)
		sc_start4(src, bl, SC_FREEZING, 50, skill_lv, 1000, src->id, 0, skill_get_time(skill_id, skill_lv));
	else
		sc_start4(src, bl, SC_BURNING, 50, skill_lv, 1000, src->id, 0, skill_get_time(skill_id, skill_lv));
	break;
case AB_ADORAMUS:
	sc_start(src, bl, SC_ADORAMUS, skill_lv * 4 + (sd ? sd->status.job_level : 50) / 2, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case WL_COMET:
	sc_start(src, bl, SC_MAGIC_POISON, 100, skill_lv, 20000);
	break;
case NPC_COMET:
	sc_start4(src, bl, SC_BURNING, 100, skill_lv, 1000, src->id, 0, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_JACKFROST:
	sc_start(src, bl, SC_FREEZE, 200, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case RA_WUGBITE:
{
	int32 wug_rate = (50 + 10 * skill_lv) + 2 * ((sd) ? pc_checkskill(sd, RA_TOOTHOFWUG) * 2 : skill_get_max(RA_TOOTHOFWUG)) - (status_get_agi(bl) / 4);
	if (wug_rate < 50)
		wug_rate = 50;
	sc_start(src, bl, SC_BITE, wug_rate, skill_lv, (skill_get_time(skill_id, skill_lv) + ((sd) ? pc_checkskill(sd, RA_TOOTHOFWUG) * 500 : skill_get_max(RA_TOOTHOFWUG))));
}
break;
case RA_SENSITIVEKEEN:
	if (rnd() % 100 < 8 * skill_lv)
		skill_castend_damage_id(src, bl, RA_WUGBITE, ((sd) ? pc_checkskill(sd, RA_WUGBITE) : skill_get_max(RA_WUGBITE)), tick, SD_ANIMATION);
	break;
case RA_FIRINGTRAP:
	sc_start4(src, bl, SC_BURNING, 50 + skill_lv * 10, skill_lv, 1000, src->id, 0, skill_get_time2(skill_id, skill_lv));
	break;
case RA_ICEBOUNDTRAP:
	sc_start(src, bl, SC_FREEZING, 50 + skill_lv * 10, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NC_PILEBUNKER:
	if (rnd() % 100 < 25 + 15 * skill_lv)
	{
		status_change_end(bl, SC_KYRIE);
		status_change_end(bl, SC_ASSUMPTIO);
		status_change_end(bl, SC_STEELBODY);
		status_change_end(bl, SC_GT_CHANGE);
		status_change_end(bl, SC_GT_REVITALIZE);
		status_change_end(bl, SC_AUTOGUARD);
		status_change_end(bl, SC_REFLECTDAMAGE);
		status_change_end(bl, SC_DEFENDER);
		status_change_end(bl, SC_PRESTIGE);
		status_change_end(bl, SC_BANDING);
		status_change_end(bl, SC_MILLENNIUMSHIELD);
	}
	break;
case NC_FLAMELAUNCHER:
	sc_start(src, bl, SC_BURNING, 20 + 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case NC_COLDSLOWER:
	// Status chances are applied officially through a check
	// The skill first trys to give the frozen status to targets that are hit
	sc_start(src, bl, SC_FREEZE, 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	if (!tsc->getSCE(SC_FREEZE)) // If it fails to give the frozen status, it will attempt to give the freezing status
		sc_start(src, bl, SC_FREEZING, 20 + skill_lv * 10, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NC_POWERSWING:
	sc_start(src, bl, SC_STUN, 10, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case GC_WEAPONCRUSH:
	skill_castend_nodamage_id(src, bl, skill_id, skill_lv, tick, BCT_ENEMY);
	break;
case LG_PINPOINTATTACK:
{
	int32 rate = 30 + 5 * ((sd) ? pc_checkskill(sd, LG_PINPOINTATTACK) : skill_lv) + (status_get_agi(src) + status_get_lv(src)) / 10;
	switch (skill_lv)
	{
	case 1:
		sc_start2(src, bl, SC_BLEEDING, rate, skill_lv, src->id, skill_get_time(skill_id, skill_lv));
		break;
	case 2:
		skill_break_equip(src, bl, EQP_HELM, rate * 100, BCT_ENEMY);
		break;
	case 3:
		skill_break_equip(src, bl, EQP_SHIELD, rate * 100, BCT_ENEMY);
		break;
	case 4:
		skill_break_equip(src, bl, EQP_ARMOR, rate * 100, BCT_ENEMY);
		break;
	case 5:
		skill_break_equip(src, bl, EQP_WEAPON, rate * 100, BCT_ENEMY);
		break;
	}
}
break;

case LG_MOONSLASHER:
	sc_start(src, src, SC_OVERBRANDREADY, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case LG_RAYOFGENESIS: // 50% chance to cause Blind on Undead and Demon monsters.
	if (battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON)
		sc_start(src, bl, SC_BLIND, 50, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case NPC_RAYOFGENESIS:
	if (skill_lv < 8)
		sc_start(src, bl, SC_BLIND, 50, skill_lv, skill_get_time(skill_id, skill_lv));
	else
		sc_start(src, bl, SC_BLIND, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case LG_HESPERUSLIT:
	if (pc_checkskill(sd, LG_PINPOINTATTACK) > 0 && sc && sc->getSCE(SC_BANDING) && sc->getSCE(SC_BANDING)->val2 > 5)
		skill_castend_damage_id(src, bl, LG_PINPOINTATTACK, rnd_value<uint16>(1, pc_checkskill(sd, LG_PINPOINTATTACK)), tick, 0);
	break;
case SR_DRAGONCOMBO:
	sc_start(src, bl, SC_STUN, 1 + skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case SR_WINDMILL:
	if (dstsd)
		skill_addtimerskill(src, tick + status_get_amotion(src), bl->id, 0, 0, skill_id, skill_lv, BF_WEAPON, 0);
	else if (dstmd)
		sc_start(src, bl, SC_STUN, 100, skill_lv, 1000 + 1000 * (rnd() % 3));
	break;
case SR_GENTLETOUCH_QUIET: //  [(Skill Level x 5) + (Caster?s DEX + Caster?s Base Level) / 10]
	sc_start(src, bl, SC_SILENCE, 5 * skill_lv + (status_get_dex(src) + status_get_lv(src)) / 10, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case SR_EARTHSHAKER:
	if (dstmd != nullptr && dstmd->guardian_data == nullptr) // Target is a mob (boss included) and not a guardian type. [Atemo]
		sc_start(src, bl, SC_EARTHSHAKER, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	sc_start(src, bl, SC_STUN, 25 + 5 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	status_change_end(bl, SC_SV_ROOTTWIST);
	break;
case SO_EARTHGRAVE:
	sc_start2(src, bl, SC_BLEEDING, 5 * skill_lv, skill_lv, src->id, skill_get_time2(skill_id, skill_lv)); // Need official rate. [LimitLine]
	break;
case SO_DIAMONDDUST:
{
	int32 rate = 5 + 5 * skill_lv;
	if (sc && sc->getSCE(SC_COOLER_OPTION))
		rate += (sd ? sd->status.job_level / 5 : 0);
	sc_start(src, bl, SC_CRYSTALIZE, rate, skill_lv, skill_get_time2(skill_id, skill_lv));
}
break;
case SO_VARETYR_SPEAR:
	sc_start(src, bl, SC_STUN, 5 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case SO_CLOUD_KILL:
	sc_start(src, bl, skill_get_sc(skill_id), 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case GN_SPORE_EXPLOSION:
	sc_start(src, bl, SC_SPORE_EXPLOSION, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case GN_SLINGITEM_RANGEMELEEATK:
	if (sd)
	{
		switch (sd->itemid)
		{ // Starting SCs here instead of do it in skill_additional_effect to simplify the code.
		case ITEMID_COCONUT_BOMB:
			sc_start(src, bl, SC_STUN, 5 + sd->status.job_level / 2, skill_lv, 1000 * sd->status.job_level / 3);
			sc_start2(src, bl, SC_BLEEDING, 3 + sd->status.job_level / 2, skill_lv, src->id, 1000 * status_get_lv(src) / 4 + sd->status.job_level / 3);
			break;
		case ITEMID_MELON_BOMB:
			sc_start4(src, bl, SC_MELON_BOMB, 100, skill_lv, 20 + sd->status.job_level, 10 + sd->status.job_level / 2, 0, 1000 * status_get_lv(src) / 4);
			break;
		case ITEMID_BANANA_BOMB:
		{
			uint16 duration = (battle_config.banana_bomb_duration ? battle_config.banana_bomb_duration : 1000 * sd->status.job_level / 4);

			sc_start(src, bl, SC_BANANA_BOMB_SITDOWN, status_get_lv(src) + sd->status.job_level + sstatus->dex / 6 - status_get_lv(bl) - tstatus->agi / 4 - tstatus->luk / 5, skill_lv, duration);
			sc_start(src, bl, SC_BANANA_BOMB, 100, skill_lv, 30000);
			break;
		}
		}
		sd->itemid = 0;
	}
	break;
case GN_HELLS_PLANT_ATK:
	sc_start(src, bl, SC_STUN, 20 + 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	sc_start2(src, bl, SC_BLEEDING, 5 + 5 * skill_lv, skill_lv, src->id, skill_get_time(skill_id, skill_lv));
	break;
case EL_WIND_SLASH: // Non confirmed rate.
	sc_start2(src, bl, SC_BLEEDING, 25, skill_lv, src->id, skill_get_time(skill_id, skill_lv));
	break;
case EL_STONE_HAMMER:
	sc_start(src, bl, SC_STUN, 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case EL_ROCK_CRUSHER:
	sc_start(src, bl, SC_ROCK_CRUSHER, 50, skill_lv, skill_get_time(EL_ROCK_CRUSHER, skill_lv));
	break;
case EL_ROCK_CRUSHER_ATK:
	sc_start(src, bl, SC_ROCK_CRUSHER_ATK, 50, skill_lv, skill_get_time(EL_ROCK_CRUSHER, skill_lv));
	break;
case EL_TYPOON_MIS:
	sc_start(src, bl, SC_SILENCE, 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case KO_JYUMONJIKIRI:
	sc_start(src, bl, SC_JYUMONJIKIRI, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case SP_SOULEXPLOSION:
case KO_SETSUDAN: // Remove soul link when hit.
	status_change_end(bl, SC_SPIRIT);
	status_change_end(bl, SC_SOULGOLEM);
	status_change_end(bl, SC_SOULSHADOW);
	status_change_end(bl, SC_SOULFALCON);
	status_change_end(bl, SC_SOULFAIRY);
	break;
case KO_MAKIBISHI:
	sc_start(src, bl, SC_STUN, 10 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case MH_EQC:
{
	struct homun_data *hd = BL_CAST(BL_HOM, src);

	if (hd)
	{
		sc_start2(src, bl, SC_STUN, 100, skill_lv, bl->id, 1000 * hd->homunculus.level / 50 + 500 * skill_lv);
		status_change_end(bl, SC_TINDER_BREAKER2);
	}
}
break;
case MH_STAHL_HORN:
	sc_start(src, bl, SC_STUN, 20 + 2 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case MH_NEEDLE_OF_PARALYZE:
	sc_start(src, bl, SC_PARALYSIS, 30 + 5 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case MH_TOXIN_OF_MANDARA:
	sc_start(src, bl, SC_TOXIN_OF_MANDARA, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case MH_XENO_SLASHER:
	sc_start4(src, bl, SC_BLEEDING, skill_lv, skill_lv, src->id, 0, 0, skill_get_time2(skill_id, skill_lv));
	break;
case NPC_MAGMA_ERUPTION:
case NC_MAGMA_ERUPTION: // Stun effect from 'slam'
	sc_start(src, bl, SC_STUN, 90, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case NC_MAGMA_ERUPTION_DOTDAMAGE: // Burning effect from 'eruption'
	sc_start4(src, bl, SC_BURNING, 10 * skill_lv, skill_lv, 1000, src->id, 0, skill_get_time2(skill_id, skill_lv));
	break;
case GN_ILLUSIONDOPING:
	if (sc_start(src, bl, SC_ILLUSIONDOPING, 100 - skill_lv * 10, skill_lv, skill_get_time(skill_id, skill_lv)))
		sc_start(src, bl, SC_HALLUCINATION, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case RL_MASS_SPIRAL:
	sc_start2(src, bl, SC_BLEEDING, 30 + 10 * skill_lv, skill_lv, src->id, skill_get_time(skill_id, skill_lv));
	break;
case RL_SLUGSHOT:
	sc_start(src, bl, SC_STUN, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case RL_BANISHING_BUSTER:
{
	if (tsc == nullptr || tsc->empty())
		break;

	if (status_isimmune(bl))
		break;

	if ((dstsd && (dstsd->class_ & MAPID_UPPERMASK) == MAPID_SOUL_LINKER) || rnd() % 100 >= 50 + 5 * skill_lv)
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		break;
	}

	uint16 n = skill_lv;

	for (const auto &it : status_db)
	{
		sc_type status = static_cast<sc_type>(it.first);

		if (n <= 0)
			break;
		if (!tsc->getSCE(status))
			continue;

		if (it.second->flag[SCF_NOBANISHINGBUSTER])
			continue;

		switch (status)
		{
		case SC_WHISTLE:
		case SC_ASSNCROS:
		case SC_POEMBRAGI:
		case SC_APPLEIDUN:
		case SC_HUMMING:
		case SC_DONTFORGETME:
		case SC_FORTUNE:
		case SC_SERVICE4U:
			if (!battle_config.dispel_song || tsc->getSCE(status)->val4 == 0)
				continue; // If in song area don't end it, even if config enabled
			break;
		case SC_ASSUMPTIO:
			if (bl->type == BL_MOB)
				continue;
			break;
		}
		if (status == SC_BERSERK || status == SC_SATURDAYNIGHTFEVER)
			tsc->getSCE(status)->val2 = 0;
		status_change_end(bl, status);
		n--;
	}
	// Remove bonus_script by Banishing Buster
	if (dstsd)
		pc_bonus_script_clear(dstsd, BSF_REM_ON_BANISHING_BUSTER);
}
break;
case RL_S_STORM:
	// kRO update 2014-02-12. Break a headgear by minimum chance 5%/10%/15%/20%/25%
	skill_break_equip(src, bl, EQP_HEAD_TOP, max(skill_lv * 500, (sstatus->dex * skill_lv * 10) - (tstatus->agi * 20)), BCT_ENEMY); //! TODO: Figure out break chance formula
	break;
case RL_AM_BLAST:
	sc_start(src, bl, SC_ANTI_M_BLAST, 20 + 10 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case SU_SCRATCH:
	sc_start2(src, bl, SC_BLEEDING, skill_lv * 10 + 70, skill_lv, src->id, skill_get_time(skill_id, skill_lv));
	break;
case SU_SV_STEMSPEAR:
	sc_start2(src, bl, SC_BLEEDING, 10, skill_lv, src->id, skill_get_time2(skill_id, skill_lv));
	break;
case SU_CN_METEOR2:
	sc_start(src, bl, SC_CURSE, 20, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case SU_SCAROFTAROU:
	sc_start(src, bl, SC_STUN, 10, skill_lv, skill_get_time2(skill_id, skill_lv)); //! TODO: What's the chance/time?
	break;
case SU_LUNATICCARROTBEAT2:
	sc_start(src, bl, SC_STUN, 20, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case SJ_FULLMOONKICK:
	sc_start(src, bl, SC_BLIND, 15 + 5 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case SJ_STAREMPEROR:
	sc_start(src, bl, SC_SILENCE, 50 + 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case SP_SHA:
	sc_start(src, bl, SC_SP_SHA, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case DK_SERVANT_W_PHANTOM:
	sc_start(src, bl, SC_HANDICAPSTATE_DEEPBLIND, 30 + 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case AG_DESTRUCTIVE_HURRICANE: // Targets hit are dealt a additional hit through Climax.
	if (sc && sc->getSCE(SC_CLIMAX) && sc->getSCE(SC_CLIMAX)->val1 == 1)
		skill_castend_damage_id(src, bl, AG_DESTRUCTIVE_HURRICANE_CLIMAX, skill_lv, tick, SD_LEVEL | SD_ANIMATION);
	break;
case AG_CRYSTAL_IMPACT: // Targets hit are dealt aftershock damage.
	skill_castend_damage_id(src, bl, AG_CRYSTAL_IMPACT_ATK, skill_lv, tick, SD_LEVEL);
	break;
case IQ_OLEUM_SANCTUM:
	sc_start(src, bl, SC_HOLY_OIL, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case IQ_FIRST_BRAND:
	sc_start(src, bl, SC_FIRST_BRAND, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case IQ_SECOND_FLAME:
case IQ_SECOND_FAITH:
case IQ_SECOND_JUDGEMENT:
	sc_start(src, bl, SC_SECOND_BRAND, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case IQ_THIRD_PUNISH:
case IQ_THIRD_FLAME_BOMB:
case IQ_THIRD_CONSECRATION:
	status_change_end(bl, SC_SECOND_BRAND);
	break;
case CD_ARBITRIUM: // Target is Deep Silenced by chance and is then dealt a 2nd splash hit.
	sc_start(src, bl, SC_HANDICAPSTATE_DEEPSILENCE, 20 + 5 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	skill_castend_damage_id(src, bl, CD_ARBITRIUM_ATK, skill_lv, tick, SD_LEVEL);
	break;
case SHC_FATAL_SHADOW_CROW:
	sc_start(src, bl, SC_DARKCROW, 100, max(1, pc_checkskill(sd, GC_DARKCROW)), skill_get_time(skill_id, skill_lv));
	break;
case ABC_UNLUCKY_RUSH:
	sc_start(src, bl, SC_HANDICAPSTATE_MISFORTUNE, 30 + 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case TR_ROSEBLOSSOM: // Rose blossom seed can only bloom if the target is hit.
	sc_start4(src, bl, SC_ROSEBLOSSOM, 100, skill_lv, TR_ROSEBLOSSOM_ATK, src->id, 0, skill_get_time(skill_id, skill_lv));
	[[fallthrough]];
case WM_METALICSOUND:
case WM_REVERBERATION:
	status_change_end(bl, SC_SOUNDBLEND);
	break;
case EM_DIAMOND_STORM:
	sc_start(src, bl, SC_HANDICAPSTATE_FROSTBITE, 5, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case EM_LIGHTNING_LAND:
	sc_start(src, bl, SC_HANDICAPSTATE_LIGHTNINGSTRIKE, 3, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case EM_VENOM_SWAMP:
	sc_start(src, bl, SC_HANDICAPSTATE_DEADLYPOISON, 3, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case EM_CONFLAGRATION:
	sc_start(src, bl, SC_HANDICAPSTATE_CONFLAGRATION, 3, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case EM_TERRA_DRIVE:
	sc_start(src, bl, SC_HANDICAPSTATE_CRYSTALLIZATION, 5, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case MT_RUSH_QUAKE:
	sc_start(src, bl, SC_RUSH_QUAKE1, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case HN_SHIELD_CHAIN_RUSH:
case HN_JACK_FROST_NOVA:
case HN_GROUND_GRAVITATION:
	sc_start(src, bl, skill_get_sc(skill_id), 100, 0, skill_get_time2(skill_id, skill_lv));
	break;
case SH_HOWLING_OF_CHUL_HO:
	sc_start(src, bl, skill_get_sc(skill_id), 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case SS_KAGEGARI:
case SS_FUUMASHOUAKU:
case SS_KUNAIWAIKYOKU:
case SS_ANTENPOU:
	sc_start(src, bl, skill_get_sc(skill_id), 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
case SS_KAGEAKUMU:
case SS_HITOUAKUMU:
case SS_ANKOKURYUUAKUMU:
	status_change_end(bl, SC_NIGHTMARE);
	break;
} // end switch skill_id

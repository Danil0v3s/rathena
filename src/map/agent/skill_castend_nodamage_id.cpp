switch (skill_id)
{
case HLIF_HEAL: //[orn]
case AL_HEAL:
case AB_HIGHNESSHEAL:
{
	int32 heal = skill_calc_heal(src, bl, skill_id, skill_lv, true);

	if (status_isimmune(bl) || (dstmd && (status_get_class(bl) == MOBID_EMPERIUM || status_get_class_(bl) == CLASS_BATTLEFIELD)))
		heal = 0;

	if (tsc != nullptr && !tsc->empty())
	{
		if (tsc->getSCE(SC_KAITE) && !status_has_mode(sstatus, MD_STATUSIMMUNE))
		{ // Bounce back heal
			if (--tsc->getSCE(SC_KAITE)->val2 <= 0)
				status_change_end(bl, SC_KAITE);
			if (src == bl)
				heal = 0; // When you try to heal yourself under Kaite, the heal is voided.
			else
			{
				bl = src;
				dstsd = sd;
			}
		}
		else if (tsc->getSCE(SC_BERSERK) || tsc->getSCE(SC_SATURDAYNIGHTFEVER))
			heal = 0; // Needed so that it actually displays 0 when healing.
	}
	if (skill_id == AL_HEAL)
		status_change_end(bl, SC_BITESCAR);
	clif_skill_nodamage(src, *bl, skill_id, heal);
	if (tsc && tsc->getSCE(SC_AKAITSUKI) && heal && skill_id != HLIF_HEAL)
		heal = ~heal + 1;
	t_exp heal_get_jobexp = status_heal(bl, heal, 0, 0);

	if (sd && dstsd && heal > 0 && sd != dstsd && battle_config.heal_exp > 0)
	{
		heal_get_jobexp = heal_get_jobexp * battle_config.heal_exp / 100;
		if (heal_get_jobexp <= 0)
			heal_get_jobexp = 1;
		pc_gainexp(sd, bl, 0, heal_get_jobexp, 0);
	}
}
break;

case CD_REPARATIO:
{
	if (bl->type != BL_PC)
	{ // Only works on players.
		if (sd)
			clif_skill_fail(*sd, skill_id);
		break;
	}

	int32 heal_amount = 0;

	if (!status_isimmune(bl))
		heal_amount = tstatus->max_hp;

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	clif_skill_nodamage(nullptr, *bl, AL_HEAL, heal_amount);
	status_heal(bl, heal_amount, 0, 0);
}
break;

case PR_REDEMPTIO:
	if (sd && !(flag & 1))
	{
		if (sd->status.party_id == 0)
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
		skill_area_temp[0] = 0;
		party_foreachsamemap(skill_area_sub,
							 sd, skill_get_splash(skill_id, skill_lv),
							 src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1,
							 skill_castend_nodamage_id);
		if (skill_area_temp[0] == 0)
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
#ifndef RENEWAL
		skill_area_temp[0] = battle_config.exp_cost_redemptio_limit - skill_area_temp[0]; // The actual penalty...
		if (skill_area_temp[0] > 0 && !map_getmapflag(src->m, MF_NOEXPPENALTY) && battle_config.exp_cost_redemptio)
		{ // Apply penalty
			// If total penalty is 1% => reduced 0.2% penalty per each revived player
			pc_lostexp(sd, u64min(sd->status.base_exp, (pc_nextbaseexp(sd) * skill_area_temp[0] * battle_config.exp_cost_redemptio / battle_config.exp_cost_redemptio_limit) / 100), 0);
		}
		status_set_sp(src, 0, 0);
#endif
		status_set_hp(src, 1, 0);
		break;
	}
	else if (!(status_isdead(*bl) && flag & 1))
	{
		// Invalid target, skip resurrection.
		break;
	}
	// Revive
	skill_area_temp[0]++; // Count it in, then fall-through to the Resurrection code.
	skill_lv = 3;		  // Resurrection level 3 is used
	[[fallthrough]];

case ALL_RESURRECTION:
	if (sd && (map_flag_gvg2(bl->m) || map_getmapflag(bl->m, MF_BATTLEGROUND)))
	{ // No reviving in WoE grounds!
		clif_skill_fail(*sd, skill_id);
		break;
	}
	if (!status_isdead(*bl))
		break;
	{
		int32 per = 0, sper = 0;
		if (tsc && tsc->getSCE(SC_HELLPOWER))
		{
			clif_skill_nodamage(src, *bl, ALL_RESURRECTION, skill_lv);
			break;
		}

		if (map_getmapflag(bl->m, MF_PVP) && dstsd && dstsd->pvp_point < 0)
			break;

		switch (skill_lv)
		{
		case 1:
			per = 10;
			break;
		case 2:
			per = 30;
			break;
		case 3:
			per = 50;
			break;
		case 4:
			per = 80;
			break;
		}
		if (dstsd && dstsd->special_state.restart_full_recover)
			per = sper = 100;
		if (status_revive(bl, per, sper))
		{
			clif_skill_nodamage(src, *bl, ALL_RESURRECTION, skill_lv); // Both Redemptio and Res show this skill-animation.
			if (sd && dstsd && battle_config.resurrection_exp > 0)
			{
				t_exp exp = 0, jexp = 0;
				int32 lv = dstsd->status.base_level - sd->status.base_level, jlv = dstsd->status.job_level - sd->status.job_level;
				if (lv > 0 && pc_nextbaseexp(dstsd))
				{
					exp = (t_exp)(dstsd->status.base_exp * lv * battle_config.resurrection_exp / 1000000.);
					if (exp < 1)
						exp = 1;
				}
				if (jlv > 0 && pc_nextjobexp(dstsd))
				{
					jexp = (t_exp)(dstsd->status.job_exp * lv * battle_config.resurrection_exp / 1000000.);
					if (jexp < 1)
						jexp = 1;
				}
				if (exp > 0 || jexp > 0)
					pc_gainexp(sd, bl, exp, jexp, 0);
			}
		}
	}
	break;

case AL_DECAGI:
case MER_DECAGI:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, (50 + skill_lv * 3 + (status_get_lv(src) + sstatus->int_) / 5), skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case AL_CRUCIS:
	if (flag & 1)
		sc_start(src, bl, type, 25 + skill_lv * 4 + status_get_lv(src) - status_get_lv(bl), skill_lv, skill_get_time(skill_id, skill_lv));
	else
	{
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR,
							  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case SP_SOULCURSE:
	if (flag & 1)
		sc_start(src, bl, type, 30 + 10 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	else
	{
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case PR_LEXDIVINA:
case MER_LEXDIVINA:
	if (tsce)
		status_change_end(bl, type);
	else
		skill_addtimerskill(src, tick + 1000, bl->id, 0, 0, skill_id, skill_lv, 100, flag);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case SA_ABRACADABRA:
	if (abra_db.empty())
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		break;
	}
	else
	{
		int32 abra_skill_id = 0, abra_skill_lv;
		size_t checked = 0, checked_max = abra_db.size() * 3;

		do
		{
			auto abra_spell = abra_db.random();

			abra_skill_id = abra_spell->skill_id;
			abra_skill_lv = min(skill_lv, skill_get_max(abra_skill_id));

			if (rnd() % 10000 < abra_spell->per[max(skill_lv - 1, 0)])
			{
				break;
			}
		} while (checked++ < checked_max);

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);

		if (sd)
		{ // player-casted
			sd->state.abra_flag = 1;
			sd->skillitem = abra_skill_id;
			sd->skillitemlv = abra_skill_lv;
			sd->skillitem_keep_requirement = false;
			clif_item_skill(sd, abra_skill_id, abra_skill_lv);
		}
		else
		{ // mob-casted
			struct unit_data *ud = unit_bl2ud(src);
			int32 inf = skill_get_inf(abra_skill_id);
			if (!ud)
				break;
			if (inf & INF_SELF_SKILL || inf & INF_SUPPORT_SKILL)
			{
				if (src->type == BL_PET)
					bl = (struct block_list *)((TBL_PET *)src)->master;
				if (!bl)
					bl = src;
				unit_skilluse_id(src, bl->id, abra_skill_id, abra_skill_lv);
			}
			else
			{ // Assume offensive skills
				int32 target_id = 0;
				if (ud->target)
					target_id = ud->target;
				else
					switch (src->type)
					{
					case BL_MOB:
						target_id = ((TBL_MOB *)src)->target_id;
						break;
					case BL_PET:
						target_id = ((TBL_PET *)src)->target_id;
						break;
					}
				if (!target_id)
					break;
				if (skill_get_casttype(abra_skill_id) == CAST_GROUND)
				{
					bl = map_id2bl(target_id);
					if (!bl)
						bl = src;
					unit_skilluse_pos(src, bl->x, bl->y, abra_skill_id, abra_skill_lv);
				}
				else
					unit_skilluse_id(src, target_id, abra_skill_id, abra_skill_lv);
			}
		}
	}
	break;

case SA_COMA:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time2(skill_id, skill_lv)));
	break;
case SA_FULLRECOVERY:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (status_isimmune(bl))
		break;
	status_percent_heal(bl, 100, 100);
	break;
case NPC_ALLHEAL:
{
	int32 heal;
	if (status_isimmune(bl))
		break;
	heal = status_percent_heal(bl, 100, 0);
	clif_skill_nodamage(nullptr, *bl, AL_HEAL, heal);
	if (dstmd)
	{ // Reset Damage Logs
		dstmd->dmglog.clear();
	}
}
break;
case SA_SUMMONMONSTER:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd)
		mob_once_spawn(sd, src->m, src->x, src->y, "--ja--", -1, 1, "", SZ_SMALL, AI_NONE);
	break;
case SA_LEVELUP:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd && pc_nextbaseexp(sd))
		pc_gainexp(sd, nullptr, pc_nextbaseexp(sd) * 10 / 100, 0, 0);
	break;
case SA_INSTANTDEATH:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_kill(src);
	break;
case SA_QUESTION:
	clif_emotion(*src, ET_QUESTION);
	[[fallthrough]];
case SA_GRAVITY:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case SA_CLASSCHANGE:
case SA_MONOCELL:
	if (dstmd)
	{
		int32 class_;

		if (sd && status_has_mode(&dstmd->status, MD_STATUSIMMUNE))
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
		class_ = (skill_id == SA_MONOCELL ? MOBID_PORING : mob_get_random_id(MOBG_CLASSCHANGE, RMF_DB_RATE, 0));
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		mob_class_change(dstmd, class_);
		if (tsc && status_has_mode(&dstmd->status, MD_STATUSIMMUNE))
		{
			const enum sc_type scs[] = {SC_QUAGMIRE, SC_PROVOKE, SC_ROKISWEIL, SC_GRAVITATION, SC_SUITON, SC_STRIPWEAPON, SC_STRIPSHIELD, SC_STRIPARMOR, SC_STRIPHELM, SC_BLADESTOP};
			for (i = SC_COMMON_MIN; i <= SC_COMMON_MAX; i++)
				if (tsc->getSCE(i))
					status_change_end(bl, (sc_type)i);
			for (i = 0; i < ARRAYLENGTH(scs); i++)
				if (tsc->getSCE(scs[i]))
					status_change_end(bl, scs[i]);
		}
	}
	break;
case SA_DEATH:
	if (sd && dstmd && status_has_mode(&dstmd->status, MD_STATUSIMMUNE))
	{
		clif_skill_fail(*sd, skill_id);
		break;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_kill(bl);
	break;
case SA_REVERSEORCISH:
case ALL_REVERSEORCISH:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
case SA_FORTUNE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd)
		pc_getzeny(sd, status_get_lv(bl) * 100, LOG_TYPE_STEAL);
	break;
case SA_TAMINGMONSTER:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd != nullptr && dstmd != nullptr)
	{
		pet_catch_process_start(*sd, 0, PET_CATCH_UNIVERSAL_ALL);
	}
	break;

case CR_PROVIDENCE:
	if (sd && dstsd)
	{ // Check they are not another crusader [Skotlex]
		if ((dstsd->class_ & MAPID_UPPERMASK) == MAPID_CRUSADER)
		{
			clif_skill_fail(*sd, skill_id);
			map_freeblock_unlock();
			return 1;
		}
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case CG_MARIONETTE:
{
	if ((sd && dstsd && (dstsd->class_ & MAPID_UPPERMASK) == MAPID_BARDDANCER && dstsd->status.sex == sd->status.sex) || (tsc && (tsc->getSCE(SC_CURSE) || tsc->getSCE(SC_QUAGMIRE))))
	{ // Cannot cast on another bard/dancer-type class of the same gender as caster, or targets under Curse/Quagmire
		clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock();
		return 1;
	}

	if (sc && tsc)
	{
		if (!sc->getSCE(SC_MARIONETTE) && !tsc->getSCE(SC_MARIONETTE2))
		{
			sc_start(src, src, SC_MARIONETTE, 100, bl->id, skill_get_time(skill_id, skill_lv));
			sc_start(src, bl, SC_MARIONETTE2, 100, src->id, skill_get_time(skill_id, skill_lv));
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		}
		else if (sc->getSCE(SC_MARIONETTE) && sc->getSCE(SC_MARIONETTE)->val1 == bl->id &&
				 tsc->getSCE(SC_MARIONETTE2) && tsc->getSCE(SC_MARIONETTE2)->val1 == src->id)
		{
			status_change_end(src, SC_MARIONETTE);
			status_change_end(bl, SC_MARIONETTE2);
		}
		else
		{
			if (sd)
				clif_skill_fail(*sd, skill_id);

			map_freeblock_unlock();
			return 1;
		}
	}
}
break;

case RG_CLOSECONFINE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start4(src, bl, type, 100, skill_lv, src->id, 0, 0, skill_get_time(skill_id, skill_lv)));
	break;
case SA_FLAMELAUNCHER: // added failure chance and chance to break weapon if turned on [Valaris]
case SA_FROSTWEAPON:
case SA_LIGHTNINGLOADER:
case SA_SEISMICWEAPON:
	if (dstsd && dstsd->status.weapon == W_FIST)
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, false);
		break;
	}
#ifdef RENEWAL
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
#else
	// 100% success rate at lv4 & 5, but lasts longer at lv5
	if (!clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, (60 + skill_lv * 10), skill_lv, skill_get_time(skill_id, skill_lv))))
	{
		if (dstsd)
		{
			int16 index = dstsd->equip_index[EQI_HAND_R];
			if (index != -1 && dstsd->inventory_data[index] && dstsd->inventory_data[index]->type == IT_WEAPON)
				pc_unequipitem(dstsd, index, 3); // Must unequip the weapon instead of breaking it [Daegaladh]
		}
		if (sd)
			clif_skill_fail(*sd, skill_id);
	}
#endif
	break;

case PR_ASPERSIO:
	if (sd && dstmd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, false);
		break;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case ITEM_ENCHANTARMS:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_get_ele(skill_id, skill_lv), skill_get_time(skill_id, skill_lv)));
	break;

case TK_SEVENWIND:
	switch (skill_get_ele(skill_id, skill_lv))
	{
	case ELE_EARTH:
		type = SC_EARTHWEAPON;
		break;
	case ELE_WIND:
		type = SC_WINDWEAPON;
		break;
	case ELE_WATER:
		type = SC_WATERWEAPON;
		break;
	case ELE_FIRE:
		type = SC_FIREWEAPON;
		break;
	case ELE_GHOST:
		type = SC_GHOSTWEAPON;
		break;
	case ELE_DARK:
		type = SC_SHADOWWEAPON;
		break;
	case ELE_HOLY:
		type = SC_ASPERSIO;
		break;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));

	sc_start(src, bl, SC_SEVENWIND, 100, skill_lv, skill_get_time(skill_id, skill_lv));

	break;

case NPC_MOVE_COORDINATE:
{
	int16 px = bl->x, py = bl->y;
	if (!skill_check_unit_movepos(0, bl, src->x, src->y, 1, 1))
	{
		return 0;
	}

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	clif_blown(bl);

	// If caster is not a boss, switch coordinates with the target
	if (status_get_class_(src) != CLASS_BOSS)
	{
		if (!skill_check_unit_movepos(0, src, px, py, 1, 1))
		{
			return 0;
		}

		clif_blown(src);
	}
}
break;

case NPC_IMMUNE_PROPERTY:
	switch (skill_lv)
	{
	case 1:
		type = SC_IMMUNE_PROPERTY_NOTHING;
		break;
	case 2:
		type = SC_IMMUNE_PROPERTY_WATER;
		break;
	case 3:
		type = SC_IMMUNE_PROPERTY_GROUND;
		break;
	case 4:
		type = SC_IMMUNE_PROPERTY_FIRE;
		break;
	case 5:
		type = SC_IMMUNE_PROPERTY_WIND;
		break;
	case 6:
		type = SC_IMMUNE_PROPERTY_DARKNESS;
		break;
	case 7:
		type = SC_IMMUNE_PROPERTY_SAINT;
		break;
	case 8:
		type = SC_IMMUNE_PROPERTY_POISON;
		break;
	case 9:
		type = SC_IMMUNE_PROPERTY_TELEKINESIS;
		break;
	case 10:
		type = SC_IMMUNE_PROPERTY_UNDEAD;
		break;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case PR_KYRIE:
case MER_KYRIE:
case SU_TUNAPARTY:
case SU_GROOMING:
case SU_CHATTERING:
case ALL_RAY_OF_PROTECTION:
	clif_skill_nodamage(bl, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
// Passive Magnum, should had been casted on yourself.
case SM_MAGNUM:
case MS_MAGNUM:
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
	break;

case MH_BLAZING_AND_FURIOUS:
case TK_JUMPKICK:
	/* Check if the target is an enemy; if not, skill should fail so the character doesn't unit_movepos (exploitable) */
	if (battle_check_target(src, bl, BCT_ENEMY) > 0)
	{
		if (unit_movepos(src, bl->x, bl->y, 2, 1))
		{
			skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
			clif_blown(src);
		}
	}
	else if (sd)
	{
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
	}
	break;

case PR_BENEDICTIO:
	if (!battle_check_undead(tstatus->race, tstatus->def_ele) && tstatus->race != RC_DEMON)
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
case AL_INCAGI:
case AL_BLESSING:
case MER_INCAGI:
case MER_BLESSING:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (dstsd != nullptr && tsc && tsc->getSCE(SC_CHANGEUNDEAD))
	{
		if (tstatus->hp > 1)
			skill_attack(BF_MISC, src, src, bl, skill_id, skill_lv, tick, flag);
		break;
	}
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case CR_REFLECTSHIELD:
case MS_REFLECTSHIELD:
	if (tsc && tsc->getSCE(SC_DARKCROW))
	{ // SC_DARKCROW prevents using reflecting skills
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
		break;
	}
	[[fallthrough]];
case PR_SLOWPOISON:
case PR_LEXAETERNA:
#ifndef RENEWAL
case PR_IMPOSITIO:
case PR_SUFFRAGIUM:
#endif
case LK_BERSERK:
case MS_BERSERK:
case KN_TWOHANDQUICKEN:
case KN_ONEHAND:
case MER_QUICKEN:
case CR_SPEARQUICKEN:
case AS_POISONREACT:
#ifndef RENEWAL
case MC_LOUD:
#endif
case MG_ENERGYCOAT:
case MO_EXPLOSIONSPIRITS:
case MO_STEELBODY:
case MO_BLADESTOP:
case LK_AURABLADE:
case LK_PARRYING:
case MS_PARRYING:
case LK_CONCENTRATION:
#ifdef RENEWAL
case HP_BASILICA:
#endif
case WS_CARTBOOST:
case SN_SIGHT:
case WS_MELTDOWN:
case WS_OVERTHRUSTMAX:
case ST_REJECTSWORD:
case HW_MAGICPOWER:
case PF_MEMORIZE:
case PA_SACRIFICE:
case PF_DOUBLECASTING:
case SG_SUN_COMFORT:
case SG_MOON_COMFORT:
case SG_STAR_COMFORT:
case GS_MADNESSCANCEL:
case GS_ADJUSTMENT:
case GS_INCREASING:
#ifdef RENEWAL
case GS_MAGICALBULLET:
#endif
case NJ_KASUMIKIRI:
case NJ_UTSUSEMI:
case NJ_NEN:
case NPC_DEFENDER:
case NPC_MAGICMIRROR:
case ST_PRESERVE:
case NPC_KEEPING:
case NPC_WEAPONBRAKER:
case NPC_BARRIER:
case NPC_INVINCIBLE:
case RK_DEATHBOUND:
case AB_EXPIATIO:
case AB_DUPLELIGHT:
case AB_SECRAMENT:
case AB_OFFERTORIUM:
case NC_ACCELERATION:
case NC_HOVERING:
case NC_SHAPESHIFT:
case WL_MARSHOFABYSS:
case WL_RECOGNIZEDSPELL:
case GC_VENOMIMPRESS:
case SC_DEADLYINFECT:
case LG_EXEEDBREAK:
case LG_PRESTIGE:
case LG_INSPIRATION:
case SR_CRESCENTELBOW:
case SR_LIGHTNINGWALK:
case GN_CARTBOOST:
case GN_BLOOD_SUCKER:
case GN_HELLS_PLANT:
case KO_MEIKYOUSISUI:
case ALL_ODINS_POWER:
case ALL_FULL_THROTTLE:
case RA_UNLIMIT:
case WL_TELEKINESIS_INTENSE:
case RL_HEAT_BARREL:
case RL_P_ALTER:
case RL_E_CHAIN:
case SU_FRESHSHRIMP:
case SU_ARCLOUSEDASH:
case NPC_MAXPAIN:
case NPC_KILLING_AURA:
case SP_SOULREAPER:
case SJ_LIGHTOFMOON:
case SJ_LIGHTOFSTAR:
case SJ_FALLINGSTAR:
case SJ_LIGHTOFSUN:
case SJ_BOOKOFDIMENSION:
case NPC_HALLUCINATIONWALK:
case DK_CHARGINGPIERCE:
case DK_VIGOR:
case AG_CLIMAX:
case IQ_POWERFUL_FAITH:
case IQ_FIRM_FAITH:
case IQ_SINCERE_FAITH:
case IQ_FIRST_FAITH_POWER:
case IQ_JUDGE:
case IQ_THIRD_EXOR_FLAME:
case IG_REBOUND_SHIELD:
case IG_HOLY_SHIELD:
case CD_ARGUTUS_VITA:
case CD_ARGUTUS_TELUM:
case CD_PRESENS_ACIES:
case CD_RELIGIO:
case CD_BENEDICTUM:
case SHC_SHADOW_EXCEED:
case SHC_POTENT_VENOM:
case SHC_ENCHANTING_SHADOW:
case MT_D_MACHINE:
case ABC_ABYSS_SLAYER:
case WH_WIND_SIGN:
case WH_CALAMITYGALE:
case BO_RESEARCHREPORT:
case TR_MYSTIC_SYMPHONY:
case TR_KVASIR_SONATA:
case EM_SPELL_ENCHANTING:
case NPC_DAMAGE_HEAL:
case NPC_RELIEVE_ON:
case NPC_RELIEVE_OFF:
case HN_BREAKINGLIMIT:
case HN_RULEBREAK:
case SH_TEMPORARY_COMMUNION:
case SKE_ENCHANTING_SKY:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case NPC_GRADUAL_GRAVITY:
case NPC_DEADLYCURSE:
	status_change_start(src, bl, type, 10000, skill_lv, 0, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case NPC_ALL_STAT_DOWN:
	status_change_start(src, bl, type, 10000, skill_lv, 0, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	break;

// EDP also give +25% WATK poison pseudo element to user.
case ASC_EDP:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
#ifdef RENEWAL
	sc_start4(src, src, SC_SUB_WEAPONPROPERTY, 100, ELE_POISON, 25, skill_id, 0, skill_get_time(skill_id, skill_lv));
#else
	sc_start4(src, src, SC_WATK_ELEMENT, 100, ELE_POISON, 25, 0, 0, skill_get_time(skill_id, skill_lv));
#endif
	break;

case LG_SHIELDSPELL:
	if (skill_lv == 1)
		type = SC_SHIELDSPELL_HP;
	else if (skill_lv == 2)
		type = SC_SHIELDSPELL_SP;
	else
		type = SC_SHIELDSPELL_ATK;

	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case DK_SERVANTWEAPON:
case ABC_FROM_THE_ABYSS:
case SOA_TALISMAN_OF_PROTECTION:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start2(src, bl, type, 100, skill_lv, src->id, skill_get_time(skill_id, skill_lv)));
	break;

case TR_SOUNDBLEND:
	skill_castend_damage_id(src, bl, skill_id, skill_lv, tick, 0);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start2(src, bl, type, 100, skill_lv, src->id, skill_get_time(skill_id, skill_lv)));
	break;

case AG_VIOLENT_QUAKE:
case AG_ALL_BLOOM:
	sc_start(src, bl, type, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;

case AG_DESTRUCTIVE_HURRICANE:
case AG_CRYSTAL_IMPACT:
	if (flag & 1)
	{ // Buff from Crystal Impact with level 1 Climax.
		sc_start(src, bl, type, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	}
	else
	{
		uint16 climax_lv = 0, splash_size = skill_get_splash(skill_id, skill_lv);

		if (sc && sc->getSCE(SC_CLIMAX))
			climax_lv = sc->getSCE(SC_CLIMAX)->val1;

		if (climax_lv == 5)
		{ // Adjusts splash AoE size depending on skill.
			if (skill_id == AG_DESTRUCTIVE_HURRICANE)
				splash_size = 9; // 19x19
			else if (skill_id == AG_CRYSTAL_IMPACT)
				splash_size = 7; // 15x15
		}

		skill_area_temp[1] = 0;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);

		if (skill_id == AG_DESTRUCTIVE_HURRICANE && climax_lv == 4) // Buff for caster instead of damage AoE.
			sc_start(src, bl, type, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
		else if (skill_id == AG_CRYSTAL_IMPACT && climax_lv == 1) // Buffs the caster and allies instead of doing damage AoE.
			map_foreachinrange(skill_area_sub, bl, splash_size, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ALLY | SD_SPLASH | 1, skill_castend_nodamage_id);
		else
		{
			if (skill_id == AG_DESTRUCTIVE_HURRICANE && climax_lv == 1) // Display extra animation for the additional hit cast.
				clif_skill_nodamage(src, *bl, AG_DESTRUCTIVE_HURRICANE_CLIMAX, skill_lv);

			map_foreachinrange(skill_area_sub, bl, splash_size, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		}
	}
	break;

case CD_MEDIALE_VOTUM:
case CD_DILECTIO_HEAL:
	if (flag & 1)
	{
		if (sd == nullptr || sd->status.party_id == 0 || (flag & 2))
		{
			int32 heal_amount = skill_calc_heal(src, bl, skill_id, skill_lv, 1);

			clif_skill_nodamage(nullptr, *bl, AL_HEAL, heal_amount);
			status_heal(bl, heal_amount, 0, 0);
		}
		else if (sd)
			party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 3, skill_castend_nodamage_id);
	}
	else
	{
		if (skill_id == CD_MEDIALE_VOTUM)
			clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
		else
		{													   // Dilectio Heal
			clif_skill_nodamage(src, *bl, skill_id, skill_lv); // Placed here to display animation on target only.
			skill_castend_nodamage_id(bl, bl, skill_id, skill_lv, tick, 1);
		}
	}
	break;

case CD_COMPETENTIA:
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
	{
		int32 hp_amount = tstatus->max_hp * (20 * skill_lv) / 100;
		int32 sp_amount = tstatus->max_sp * (20 * skill_lv) / 100;

		clif_skill_nodamage(nullptr, *bl, AL_HEAL, hp_amount);
		status_heal(bl, hp_amount, 0, 0);

		clif_skill_nodamage(nullptr, *bl, MG_SRECOVERY, sp_amount);
		status_heal(bl, 0, sp_amount, 0);

		clif_skill_nodamage(bl, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	break;

case BO_ADVANCE_PROTECTION:
	if (sd && (!dstsd || pc_checkequip(dstsd, EQP_SHADOW_GEAR) < 0))
	{
		clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock(); // Don't consume item requirements
		return 0;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case EM_ACTIVITY_BURN:
	if (bl->type == BL_PC && rnd() % 100 < 20 + 10 * skill_lv)
	{
		uint8 ap_burn[5] = {20, 30, 50, 60, 70};

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		status_zap(bl, 0, 0, ap_burn[skill_lv - 1]);
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
	break;

case EM_INCREASING_ACTIVITY:
	if (bl->type == BL_PC)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		status_heal(bl, 0, 0, 10 * skill_lv, 0);
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
	break;

case SJ_GRAVITYCONTROL:
{
	int32 fall_damage = sstatus->batk + sstatus->rhw.atk - tstatus->def2;

	if (bl->type == BL_PC)
		fall_damage += dstsd->weight / 10 - tstatus->def;
	else // Monster's don't have weight. Put something in its place.
		fall_damage += 50 * status_get_lv(src) - tstatus->def;

	fall_damage = max(1, fall_damage);

	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start2(src, bl, type, 100, skill_lv, fall_damage, skill_get_time(skill_id, skill_lv)));
}
break;
case NPC_HALLUCINATION:
case NPC_HELLPOWER:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, skill_lv * 20, skill_lv, skill_get_time2(skill_id, skill_lv)));
	break;

case KN_AUTOCOUNTER:
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	skill_addtimerskill(src, tick + 100, bl->id, 0, 0, skill_id, skill_lv, BF_WEAPON, flag);
	break;

case SO_STRIKING:
	if (battle_check_target(src, bl, BCT_SELF | BCT_PARTY) > 0)
	{
		int32 bonus = 0;

		if (dstsd)
		{
			int16 index = dstsd->equip_index[EQI_HAND_R];

			if (index >= 0 && dstsd->inventory_data[index] && dstsd->inventory_data[index]->type == IT_WEAPON)
				bonus = (20 * skill_lv) * dstsd->inventory_data[index]->weapon_level;
		}

		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start2(src, bl, type, 100, skill_lv, bonus, skill_get_time(skill_id, skill_lv)));
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL_TOTARGET);
	break;

case NPC_STOP:
	if (clif_skill_nodamage(src, *bl, skill_id, skill_lv,
							sc_start2(src, bl, type, 100, skill_lv, src->id, skill_get_time(skill_id, skill_lv))))
		sc_start2(src, src, type, 100, skill_lv, bl->id, skill_get_time(skill_id, skill_lv));
	break;
case HP_ASSUMPTIO:
	if (sd && dstmd)
		clif_skill_fail(*sd, skill_id);
	else
		clif_skill_nodamage(src, *bl, skill_id, skill_lv,
							sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
case MG_SIGHT:
case MER_SIGHT:
case AL_RUWACH:
case WZ_SIGHTBLASTER:
case NPC_WIDESIGHT:
case NPC_STONESKIN:
case NPC_ANTIMAGIC:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start2(src, bl, type, 100, skill_lv, skill_id, skill_get_time(skill_id, skill_lv)));
	break;
case HLIF_AVOID:
case HAMI_DEFENCE:
	// Master
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	// Homunculus
	clif_skill_nodamage(src, *src, skill_id, skill_lv, sc_start(src, src, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
case NJ_BUNSINJYUTSU:
	status_change_end(bl, SC_BUNSINJYUTSU); // on official recasting cancels existing mirror image [helvetica]
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	status_change_end(bl, SC_NEN);
	break;
	/* Was modified to only affect targetted char.	[Skotlex]
		case HP_ASSUMPTIO:
			if (flag&1)
				sc_start(bl,type,100,skill_lv,skill_get_time(skill_id,skill_lv));
			else
			{
				map_foreachinallrange(skill_area_sub, bl,
					skill_get_splash(skill_id, skill_lv), BL_PC,
					src, skill_id, skill_lv, tick, flag|BCT_ALL|1,
					skill_castend_nodamage_id);
				clif_skill_nodamage(src,*bl,skill_id,skill_lv);
			}
			break;
	*/
case SM_ENDURE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case AS_ENCHANTPOISON:
	if (sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, false);

		if (sd != nullptr)
		{
			clif_skill_fail(*sd, skill_id);
		}
	}
	break;

case LK_TENSIONRELAX:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start4(src, bl, type, 100, skill_lv, 0, 0, skill_get_time2(skill_id, skill_lv),
								  skill_get_time(skill_id, skill_lv)));
	break;

case MC_CHANGECART:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case MC_CARTDECORATE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd)
	{
		clif_SelectCart(sd);
	}
	break;

case TK_MISSION:
	if (sd)
	{
		if (sd->mission_mobid && (sd->mission_count || rnd() % 100))
		{ // Cannot change target when already have one
			clif_mission_info(sd, sd->mission_mobid, sd->mission_count);
			clif_skill_fail(*sd, skill_id);
			break;
		}

		int32 id = mob_get_random_id(MOBG_TAEKWON_MISSION, RMF_NONE, 0);

		if (!id)
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
		sd->mission_mobid = id;
		sd->mission_count = 0;
		pc_setglobalreg(sd, add_str(TKMISSIONID_VAR), id);
		clif_mission_info(sd, id, 0);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case AC_CONCENTRATION:
{
	int32 splash = skill_get_splash(skill_id, skill_lv);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	skill_reveal_trap_inarea(src, splash, src->x, src->y);
	map_foreachinallrange(status_change_timer_sub, src,
						  splash, BL_CHAR, src, nullptr, type, tick);
}
break;

case SM_PROVOKE:
case SM_SELFPROVOKE:
case MER_PROVOKE:
	if (status_has_mode(tstatus, MD_STATUSIMMUNE) || battle_check_undead(tstatus->race, tstatus->def_ele))
	{
		map_freeblock_unlock();
		return 1;
	}
	// Official chance is 70% + 3%*skill_lv + srcBaseLevel% - tarBaseLevel%
	if (!(i = sc_start(src, bl, type, skill_id == SM_SELFPROVOKE ? 100 : (70 + 3 * skill_lv + status_get_lv(src) - status_get_lv(bl)), skill_lv, skill_get_time(skill_id, skill_lv))))
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock();
		return 0;
	}
	clif_skill_nodamage(src, *bl, skill_id == SM_SELFPROVOKE ? SM_PROVOKE : skill_id, skill_lv, i != 0);
	unit_skillcastcancel(bl, 2);

	if (dstmd)
	{
		dstmd->state.provoke_flag = src->id;
		mob_target(dstmd, src, skill_get_range2(src, skill_id, skill_lv, true));
	}
	// Provoke can cause Coma even though it's a nodamage skill
	if (sd && battle_check_coma(*sd, *bl, BF_MISC))
		status_change_start(src, bl, SC_COMA, 10000, skill_lv, 0, src->id, 0, 0, SCSTART_NONE);
	break;

case ML_DEVOTION:
case CR_DEVOTION:
{
	int32 count, lv;
	if (!dstsd || (!sd && !mer))
	{ // Only players can be devoted
		if (sd)
			clif_skill_fail(*sd, skill_id);
		break;
	}

	if ((lv = status_get_lv(src) - dstsd->status.base_level) < 0)
		lv = -lv;
	if (lv > battle_config.devotion_level_difference ||						   // Level difference requeriments
		(dstsd->sc.getSCE(type) && dstsd->sc.getSCE(type)->val1 != src->id) || // Cannot Devote a player devoted from another source
		(skill_id == ML_DEVOTION && (!mer || mer != dstsd->md)) ||			   // Mercenary only can devote owner
		(dstsd->class_ & MAPID_UPPERMASK) == MAPID_CRUSADER ||				   // Crusader Cannot be devoted
		(dstsd->sc.getSCE(SC_HELLPOWER)))									   // Players affected by SC_HELLPOWER cannot be devoted.
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock();
		return 1;
	}

	i = 0;
	count = (sd) ? min(skill_lv, MAX_DEVOTION) : 1; // Mercenary only can Devote owner
	if (sd)
	{ // Player Devoting Player
		ARR_FIND(0, count, i, sd->devotion[i] == bl->id);
		if (i == count)
		{
			ARR_FIND(0, count, i, sd->devotion[i] == 0);
			if (i == count)
			{ // No free slots, skill Fail
				clif_skill_fail(*sd, skill_id);
				map_freeblock_unlock();
				return 1;
			}
		}

		sd->devotion[i] = bl->id;
	}
	else
		mer->devotion_flag = 1; // Mercenary Devoting Owner

	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start4(src, bl, type, 10000, src->id, i, skill_get_range2(src, skill_id, skill_lv, true), 0, skill_get_time2(skill_id, skill_lv)));
	clif_devotion(src, nullptr);
}
break;

case SP_SOULUNITY:
{
	int8 count = min(5 + skill_lv, MAX_UNITED_SOULS);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
	{
		if (!dstsd || !sd)
		{ // Only put player's souls in unity.
			if (sd)
				clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
			break;
		}

		if (dstsd->sc.getSCE(type) && dstsd->sc.getSCE(type)->val2 != src->id)
		{ // Fail if a player is in unity with another source.
			if (sd)
				clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
			map_freeblock_unlock();
			return 1;
		}

		if (sd)
		{ // Unite player's soul with caster's soul.
			i = 0;

			ARR_FIND(0, count, i, sd->united_soul[i] == bl->id);
			if (i == count)
			{
				ARR_FIND(0, count, i, sd->united_soul[i] == 0);
				if (i == count)
				{ // No more free slots? Fail the skill.
					clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
					map_freeblock_unlock();
					return 1;
				}
			}

			sd->united_soul[i] = bl->id;
		}

		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start4(src, bl, type, 100, skill_lv, src->id, i, 0, skill_get_time(skill_id, skill_lv)));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
}
break;

case DK_SERVANT_W_SIGN: // Max allowed targets to be marked.
	// Only players and monsters can be marked....I think??? [Rytech]
	// Lets only allow players and monsters to use this skill for safety reasons.
	if ((!dstsd && !dstmd) || !sd && !md)
	{
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
		break;
	}

	// Check if the target is already marked by another source.
	if (tsc && tsc->getSCE(type) && tsc->getSCE(type)->val1 != src->id)
	{
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
		map_freeblock_unlock();
		return 1;
	}

	// Mark the target.
	if (sd)
	{
		int8 count = MAX_SERVANT_SIGN;

		ARR_FIND(0, count, i, sd->servant_sign[i] == bl->id);
		if (i == count)
		{
			ARR_FIND(0, count, i, sd->servant_sign[i] == 0);
			if (i == count)
			{ // Max number of targets marked. Fail the skill.
				clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
				map_freeblock_unlock();
				return 1;
			}

			// Add the ID of the marked target to the player's sign list.
			sd->servant_sign[i] = bl->id;
		}

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		sc_start4(src, bl, type, 100, src->id, i, skill_lv, 0, skill_get_time(skill_id, skill_lv));
	}
	else if (md) // Monster's cant track with this skill. Just give the status.
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start4(src, bl, type, 100, 0, 0, skill_lv, 0, skill_get_time(skill_id, skill_lv)));
	break;

case MO_CALLSPIRITS:
	if (sd)
	{
		int32 limit = skill_lv;
		if (sd->sc.getSCE(SC_RAISINGDRAGON))
			limit += sd->sc.getSCE(SC_RAISINGDRAGON)->val1;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		pc_addspiritball(sd, skill_get_time(skill_id, skill_lv), limit);
	}
	break;

case CH_SOULCOLLECT:
	if (sd)
	{
		int32 limit = 5;
		if (sd->sc.getSCE(SC_RAISINGDRAGON))
			limit += sd->sc.getSCE(SC_RAISINGDRAGON)->val1;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		for (i = 0; i < limit; i++)
			pc_addspiritball(sd, skill_get_time(skill_id, skill_lv), limit);
	}
	break;

case MO_KITRANSLATION:
	if (dstsd && ((dstsd->class_ & MAPID_BASEMASK) != MAPID_GUNSLINGER && (dstsd->class_ & MAPID_UPPERMASK) != MAPID_REBELLION) && dstsd->spiritball < 5)
	{
		// Require will define how many spiritballs will be transferred
		struct s_skill_condition require;
		require = skill_get_requirement(sd, skill_id, skill_lv);
		pc_delspiritball(sd, require.spiritball, 0);
		for (i = 0; i < require.spiritball; i++)
			pc_addspiritball(dstsd, skill_get_time(skill_id, skill_lv), 5);
	}
	else
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock();
		return 0;
	}
	break;

case TK_TURNKICK:
case MO_BALKYOUNG: // Passive part of the attack. Splash knock-back+stun. [Skotlex]
	if (skill_area_temp[1] != bl->id)
	{
		skill_blown(src, bl, skill_get_blewcount(skill_id, skill_lv), -1, BLOWN_NONE);
		skill_additional_effect(src, bl, skill_id, skill_lv, BF_MISC, ATK_DEF, tick); // Use Misc rather than weapon to signal passive pushback
	}
	break;

case MO_ABSORBSPIRITS:
	i = 0;
	if (dstsd && (battle_check_target(src, bl, BCT_SELF) > 0 || (battle_check_target(src, bl, BCT_ENEMY) > 0 && (map_flag_vs(src->m) || (sd && sd->duel_group && sd->duel_group == dstsd->duel_group)))) && // Only works on self and enemies
		((dstsd->class_ & MAPID_BASEMASK) != MAPID_GUNSLINGER || (dstsd->class_ & MAPID_UPPERMASK) != MAPID_REBELLION))
	{ // split the if for readability, and included gunslingers in the check so that their coins cannot be removed [Reddozen]
		if (dstsd->spiritball > 0)
		{
			i = dstsd->spiritball * 7;
			pc_delspiritball(dstsd, dstsd->spiritball, 0);
		}
		if (dstsd->spiritcharm_type != CHARM_TYPE_NONE && dstsd->spiritcharm > 0)
		{
			i += dstsd->spiritcharm * 7;
			pc_delspiritcharm(dstsd, dstsd->spiritcharm, dstsd->spiritcharm_type);
		}
	}
	else if (dstmd && !status_has_mode(tstatus, MD_STATUSIMMUNE) && rnd() % 100 < 20)
	{ // check if target is a monster and not status immune, for the 20% chance to absorb 2 SP per monster's level [Reddozen]
		i = 2 * dstmd->level;
		mob_target(dstmd, src, 0);
	}
	else
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		break;
	}
	if (i)
		status_heal(src, 0, i, 3);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, i != 0);
	break;

case AC_MAKINGARROW:
	if (sd != nullptr)
	{
		clif_arrow_create_list(*sd);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case AM_PHARMACY:
	if (sd)
	{
		clif_skill_produce_mix_list(*sd, skill_id, 22);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case SA_CREATECON:
	if (sd != nullptr)
	{
		clif_elementalconverter_list(*sd);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case BS_HAMMERFALL:
	skill_addtimerskill(src, tick + 1000, bl->id, 0, 0, skill_id, skill_lv, min(20 + 10 * skill_lv, 50 + 5 * skill_lv), flag);
	break;

case RG_RAID:
	skill_area_temp[1] = 0;
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_foreachinrange(skill_area_sub, bl,
					   skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL,
					   src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1,
					   skill_castend_damage_id);
	status_change_end(src, SC_HIDING);
	break;

// List of self skills that give damage around caster
case ASC_METEORASSAULT:
case GS_SPREADATTACK:
case RK_WINDCUTTER:
case RK_STORMBLAST:
case NC_AXETORNADO:
case GC_COUNTERSLASH:
case SR_SKYNETBLOW:
case SR_RAMPAGEBLASTER:
case SR_HOWLINGOFLION:
case LG_CANNONSPEAR:
case LG_OVERBRAND:
case NPC_RAYOFGENESIS:
case LG_RAYOFGENESIS:
case MH_THE_ONE_FIGHTER_RISES:
case MH_HEILIGE_PFERD:
case KO_HAPPOKUNAI:
case RL_FIREDANCE:
case RL_R_TRIP:
case SJ_FULLMOONKICK:
case SJ_NEWMOONKICK:
case SJ_SOLARBURST:
case SJ_STAREMPEROR:
case SJ_FALLINGSTAR_ATK:
case DK_SERVANT_W_DEMOL:
case AG_FROZEN_SLASH:
case IQ_OLEUM_SANCTUM:
case IQ_MASSIVE_F_BLASTER:
case IQ_EXPOSION_BLASTER:
case SHC_IMPACT_CRATER:
case MT_AXE_STOMP:
case MT_MIGHTY_SMASH:
case ABC_ABYSS_DAGGER:
case BO_EXPLOSIVE_POWDER:
case SOA_EXORCISM_OF_MALICIOUS_SOUL:
case SOA_TALISMAN_OF_WHITE_TIGER:
case SKE_DAWN_BREAK:
case SKE_RISING_MOON:
case SKE_MIDNIGHT_KICK:
{
	int32 starget = BL_CHAR | BL_SKILL;

	if (skill_id == SR_HOWLINGOFLION)
		starget = splash_target(src);
	if (skill_id == SJ_NEWMOONKICK)
	{
		if (tsce)
		{
			status_change_end(bl, type);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			break;
		}
		else
			sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	if (skill_id == SJ_STAREMPEROR && sc && sc->getSCE(SC_DIMENSION))
	{
		if (sd)
		{
			// Remove old shields if any exist.
			pc_delspiritball(sd, sd->spiritball, 0);
			sc_start2(src, bl, SC_DIMENSION1, 100, skill_lv, status_get_max_sp(src), skill_get_time2(SJ_BOOKOFDIMENSION, 1));
			sc_start2(src, bl, SC_DIMENSION2, 100, skill_lv, status_get_max_sp(src), skill_get_time2(SJ_BOOKOFDIMENSION, 1));
		}
		status_change_end(src, SC_DIMENSION);
	}
	if (skill_id == IQ_MASSIVE_F_BLASTER || skill_id == SHC_IMPACT_CRATER || skill_id == MT_AXE_STOMP || skill_id == ABC_ABYSS_DAGGER)
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	if (skill_id == MH_THE_ONE_FIGHTER_RISES)
	{
		hom_addspiritball(hd, MAX_SPIRITBALL);
	}

	// TODO: refactor the ifs above into the switch below

	switch (skill_id)
	{
	case SOA_EXORCISM_OF_MALICIOUS_SOUL:
		if (sd != nullptr)
		{
			// Remove old souls if any exist.
			sd->soulball_old = sd->soulball;
			pc_delsoulball(*sd, sd->soulball, 0);
		}
		break;

	case SOA_TALISMAN_OF_WHITE_TIGER:
		if (sc != nullptr && sc->getSCE(SC_T_FIRST_GOD) != nullptr)
		{
			sc_start(src, src, skill_get_sc(skill_id), 100, skill_lv, skill_get_time(skill_id, skill_lv));
		}
		break;

	case SKE_RISING_MOON:
		if (sc == nullptr || (sc->getSCE(SC_RISING_MOON) == nullptr && sc->getSCE(SC_MIDNIGHT_MOON) == nullptr && sc->getSCE(SC_DAWN_MOON) == nullptr))
		{
			sc_start(src, src, SC_RISING_MOON, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		}
		else if (sc->getSCE(SC_MIDNIGHT_MOON) == nullptr && sc->getSCE(SC_DAWN_MOON) == nullptr)
		{
			sc_start(src, src, SC_MIDNIGHT_MOON, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		}
		else if (sc->getSCE(SC_DAWN_MOON) == nullptr)
		{
			sc_start(src, src, SC_DAWN_MOON, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		}
		else if (sc->getSCE(SC_RISING_SUN) != nullptr)
		{
			status_change_end(bl, SC_DAWN_MOON);
		}
		break;
	}

	skill_area_temp[1] = 0;
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	i = map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), starget,
						   src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	if (!i && (skill_id == RK_WINDCUTTER || skill_id == NC_AXETORNADO || skill_id == LG_CANNONSPEAR || skill_id == SR_SKYNETBLOW || skill_id == KO_HAPPOKUNAI))
		clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
}
break;

case SHC_DANCING_KNIFE:
	if (flag & 1)
	{
		skill_area_temp[1] = 0;

		// Note: doesn't force player to stand before attacking
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_LEVEL | SD_SPLASH, skill_castend_damage_id);
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	break;

case MT_A_MACHINE:
	if (flag & 1)
	{
		skill_area_temp[1] = 0;

		if (sd && pc_issit(sd))
		{ // Force player to stand before attacking
			pc_setstand(sd, true);
			skill_sit(sd, false);
		}

		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_LEVEL | SD_SPLASH, skill_castend_damage_id);
	}
	else
	{
		if (dstsd)
		{
			int32 lv = abs(status_get_lv(src) - status_get_lv(bl));

			if (lv > battle_config.attack_machine_level_difference)
			{
				if (sd)
					clif_skill_fail(*sd, skill_id);
				map_freeblock_unlock();
				return 0;
			}
		}

		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	break;

case EM_ELEMENTAL_BUSTER:
{
	if (sd == nullptr)
		break;

	if (!sd->ed || !(sd->ed->elemental.class_ >= ELEMENTALID_DILUVIO && sd->ed->elemental.class_ <= ELEMENTALID_SERPENS))
	{
		clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock();
		return 0;
	}

	uint16 buster_element;

	switch (sd->ed->elemental.class_)
	{
	case ELEMENTALID_ARDOR:
		buster_element = EM_ELEMENTAL_BUSTER_FIRE;
		break;
	case ELEMENTALID_DILUVIO:
		buster_element = EM_ELEMENTAL_BUSTER_WATER;
		break;
	case ELEMENTALID_PROCELLA:
		buster_element = EM_ELEMENTAL_BUSTER_WIND;
		break;
	case ELEMENTALID_TERREMOTUS:
		buster_element = EM_ELEMENTAL_BUSTER_GROUND;
		break;
	case ELEMENTALID_SERPENS:
		buster_element = EM_ELEMENTAL_BUSTER_POISON;
		break;
	}

	skill_area_temp[1] = 0;
	clif_skill_nodamage(src, *bl, buster_element, skill_lv); // Animation for the triggered blaster element.
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);		 // Triggered after blaster animation to make correct skill name scream appear.
	map_foreachinrange(skill_area_sub, bl, 6, BL_CHAR | BL_SKILL, src, buster_element, skill_lv, tick, flag | BCT_ENEMY | SD_LEVEL | SD_SPLASH | 1, skill_castend_damage_id);
}
break;

case NPC_IGNITIONBREAK:
case RK_IGNITIONBREAK:
	skill_area_temp[1] = 0;
#if PACKETVER >= 20180207
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
#else
	clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
#endif
	map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	break;

case SR_TIGERCANNON:
case SR_WINDMILL:
case GN_CART_TORNADO:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	[[fallthrough]];
case SR_EARTHSHAKER:
case NC_INFRAREDSCAN:
case NPC_VAMPIRE_GIFT:
case NPC_HELLJUDGEMENT:
case NPC_HELLJUDGEMENT2:
case NPC_PULSESTRIKE:
case LG_MOONSLASHER:
case NPC_WIDECRITICALWOUND:
	skill_castend_damage_id(src, src, skill_id, skill_lv, tick, flag);
	break;

#ifdef RENEWAL
case KN_BRANDISHSPEAR:
	map_foreachindir(skill_area_sub, src->m, src->x, src->y, bl->x, bl->y,
					 skill_get_splash(skill_id, skill_lv), skill_get_maxcount(skill_id, skill_lv), 0, splash_target(src),
					 src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 0,
					 skill_castend_damage_id);
	break;
#else
case KN_BRANDISHSPEAR:
#endif
case ML_BRANDISH:
	skill_area_temp[1] = bl->id;

	if (skill_lv >= 10)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, bl->x, bl->y,
						 skill_get_splash(skill_id, skill_lv), 1, skill_get_maxcount(skill_id, skill_lv) - 1, splash_target(src),
						 src, skill_id, skill_lv, tick, flag | BCT_ENEMY | (sd ? 3 : 0),
						 skill_castend_damage_id);
	if (skill_lv >= 7)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, bl->x, bl->y,
						 skill_get_splash(skill_id, skill_lv), 1, skill_get_maxcount(skill_id, skill_lv) - 2, splash_target(src),
						 src, skill_id, skill_lv, tick, flag | BCT_ENEMY | (sd ? 2 : 0),
						 skill_castend_damage_id);
	if (skill_lv >= 4)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, bl->x, bl->y,
						 skill_get_splash(skill_id, skill_lv), 1, skill_get_maxcount(skill_id, skill_lv) - 3, splash_target(src),
						 src, skill_id, skill_lv, tick, flag | BCT_ENEMY | (sd ? 1 : 0),
						 skill_castend_damage_id);
	map_foreachindir(skill_area_sub, src->m, src->x, src->y, bl->x, bl->y,
					 skill_get_splash(skill_id, skill_lv), skill_get_maxcount(skill_id, skill_lv) - 3, 0, splash_target(src),
					 src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 0,
					 skill_castend_damage_id);
	break;

case WZ_SIGHTRASHER:
	// Passive side of the attack.
	status_change_end(src, SC_SIGHT);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_foreachinshootrange(skill_area_sub, src,
							skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL,
							src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_ANIMATION | 1,
							skill_castend_damage_id);
	break;

case WZ_FROSTNOVA:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_area_temp[1] = 0;
	map_foreachinshootrange(skill_attack_area, src,
							skill_get_splash(skill_id, skill_lv), splash_target(src),
							BF_MAGIC, src, src, skill_id, skill_lv, tick, flag, BCT_ENEMY);
	break;

case NPC_SELFDESTRUCTION:
	// Self Destruction hits everyone in range (allies+enemies)
	// Except for Summoned Marine spheres on non-versus maps, where it's just enemies and your own slaves.
	if ((md == nullptr || md->special_state.ai == AI_SPHERE) && !map_flag_vs(src->m))
	{
		// Enable Marine Spheres to damage own Homunculus and summons outside PVP
		if (battle_config.alchemist_summon_setting & 8)
			i = BCT_ENEMY | BCT_SLAVE;
		else
			i = BCT_ENEMY;
	}
	else
	{
		i = BCT_ALL;
	}
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
	map_delblock(src); // Required to prevent chain-self-destructions hitting back.
	map_foreachinshootrange(skill_area_sub, bl,
							skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL,
							src, skill_id, skill_lv, tick, flag | i,
							skill_castend_damage_id);
	if (map_addblock(src))
	{
		map_freeblock_unlock();
		return 1;
	}
	// Won't display the damage, but drop items and give exp
	status_zap(src, sstatus->hp, 0, 0);
	break;
case AL_ANGELUS:
#ifdef RENEWAL
case PR_SUFFRAGIUM:
case PR_IMPOSITIO:
#endif
case PR_MAGNIFICAT:
case PR_GLORIA:
case SOA_SOUL_OF_HEAVEN_AND_EARTH:
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
	{

		// Animations don't play when outside visible range
		if (check_distance_bl(src, bl, AREA_SIZE))
			clif_skill_nodamage(bl, *bl, skill_id, skill_lv);

		if (skill_id == SOA_SOUL_OF_HEAVEN_AND_EARTH)
		{
			status_percent_heal(bl, 0, 100);

			if (src != bl && sc != nullptr && sc->getSCE(SC_TOTEM_OF_TUTELARY) != nullptr)
			{
				status_heal(bl, 0, 0, 3 * skill_lv, 0);
			}
		}

		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	break;
#ifdef RENEWAL
case MC_LOUD:
#endif
case SN_WINDWALK:
case CASH_BLESSING:
case CASH_INCAGI:
case CASH_ASSUMPTIO:
case WM_FRIGG_SONG:
case NV_HELPANGEL:
case IG_GUARDIAN_SHIELD:
case IG_ULTIMATE_SACRIFICE: // Is the animation on this skill correct? Check if its on caster only or all affected. [Rytech]
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
		clif_skill_nodamage(bl, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	else if (sd)
	{
		if (skill_id == IG_ULTIMATE_SACRIFICE)
			status_set_hp(src, 1, 0);
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
	break;

case MER_MAGNIFICAT:
	if (mer != nullptr)
	{
		clif_skill_nodamage(bl, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
		if (mer->master && mer->master->status.party_id != 0 && !(flag & 1))
			party_foreachsamemap(skill_area_sub, mer->master, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
		else if (mer->master && !(flag & 1))
			clif_skill_nodamage(src, *mer->master, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	break;

case BS_ADRENALINE:
case BS_ADRENALINE2:
case BS_WEAPONPERFECT:
case BS_OVERTHRUST:
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
	{
		int32 weapontype = skill_get_weapontype(skill_id);
		if (!weapontype || !dstsd || pc_check_weapontype(dstsd, weapontype))
		{
			clif_skill_nodamage(bl, *bl, skill_id, skill_lv,
								sc_start2(src, bl, type, 100, skill_lv, (src == bl) ? 1 : 0, skill_get_time(skill_id, skill_lv)));
		}
	}
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub,
							 sd, skill_get_splash(skill_id, skill_lv),
							 src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1,
							 skill_castend_nodamage_id);
	}
	break;

case BS_MAXIMIZE:
case NV_TRICKDEAD:
case CR_DEFENDER:
case ML_DEFENDER:
case CR_AUTOGUARD:
case ML_AUTOGUARD:
case TK_READYSTORM:
case TK_READYDOWN:
case TK_READYTURN:
case TK_READYCOUNTER:
case TK_DODGE:
case CR_SHRINK:
case SG_FUSION:
case GS_GATLINGFEVER:
case SJ_LUNARSTANCE:
case SJ_STARSTANCE:
case SJ_UNIVERSESTANCE:
case SJ_SUNSTANCE:
case SP_SOULCOLLECT:
case IG_GUARD_STANCE:
case IG_ATTACK_STANCE:
	if (tsce)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, status_change_end(bl, type));
		map_freeblock_unlock();
		return 0;
	}

	if (skill_id == SP_SOULCOLLECT)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start2(src, bl, type, 100, skill_lv, pc_checkskill(sd, SP_SOULENERGY), skill_get_time(skill_id, skill_lv)));
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	break;
case SL_KAITE:
case SL_KAAHI:
case SL_KAIZEL:
case SL_KAUPE:
case SP_KAUTE:
	if (sd)
	{
		if (!dstsd || !(
						  (sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_SOULLINKER) ||
						  (dstsd->class_ & MAPID_UPPERMASK) == MAPID_SOUL_LINKER ||
						  dstsd->status.char_id == sd->status.char_id ||
						  dstsd->status.char_id == sd->status.partner_id ||
						  dstsd->status.char_id == sd->status.child ||
						  (skill_id == SP_KAUTE && dstsd->sc.getSCE(SC_SOULUNITY))))
		{
			status_change_start(src, src, SC_STUN, 10000, skill_lv, 0, 0, 0, 500, SCSTART_NORATEDEF);
			clif_skill_fail(*sd, skill_id);
			break;
		}
	}
	if (skill_id == SP_KAUTE)
	{
		if (!status_charge(src, sstatus->max_hp * (10 + 2 * skill_lv) / 100, 0))
		{
			if (sd)
				clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
			break;
		}
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		status_heal(bl, 0, tstatus->max_sp * (10 + 2 * skill_lv) / 100, 2);
	}
	else
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
case SM_AUTOBERSERK:
case MER_AUTOBERSERK:
	if (tsce)
		i = status_change_end(bl, type);
	else
		i = sc_start(src, bl, type, 100, skill_lv, 60000);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, i);
	break;
case TF_HIDING:
case ST_CHASEWALK:
case KO_YAMIKUMO:
	if (tsce)
	{
		clif_skill_nodamage(src, *bl, skill_id, -1, status_change_end(bl, type)); // Hide skill-scream animation.
		map_freeblock_unlock();
		return 0;
	}
	clif_skill_nodamage(src, *bl, skill_id, -1, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
case TK_RUN:
	if (tsce)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, status_change_end(bl, type));
		map_freeblock_unlock();
		return 0;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start4(src, bl, type, 100, skill_lv, unit_getdir(bl), 0, 0, 0));
	if (sd)				  // If the client receives a skill-use packet inmediately before a walkok packet, it will discard the walk packet! [Skotlex]
		clif_walkok(*sd); // So aegis has to resend the walk ok.
	break;

case AS_CLOAKING:
case GC_CLOAKINGEXCEED:
case LG_FORCEOFVANGUARD:
case SC_REPRODUCE:
case SC_INVISIBILITY:
case RA_CAMOUFLAGE:
	if (tsce)
	{
		i = status_change_end(bl, type);
		if (i)
			clif_skill_nodamage(src, *bl, skill_id, (skill_id == LG_FORCEOFVANGUARD || skill_id == RA_CAMOUFLAGE) ? skill_lv : -1, i);
		else if (sd)
			clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock();
		return 0;
	}
	i = sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	if (i)
		clif_skill_nodamage(src, *bl, skill_id, (skill_id == LG_FORCEOFVANGUARD || skill_id == RA_CAMOUFLAGE) ? skill_lv : -1, i);
	else if (sd)
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL_LEVEL);
	break;

case CG_SPECIALSINGER:
	if (tsc && tsc->getSCE(SC_ENSEMBLEFATIGUE))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		status_change_end(bl, SC_ENSEMBLEFATIGUE);
	}
	break;

case BD_ADAPTATION:
#ifdef RENEWAL
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
#else
	if (tsc && tsc->getSCE(SC_DANCING))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		status_change_end(bl, SC_DANCING);
	}
#endif
	break;

case BA_FROSTJOKER:
case DC_SCREAM:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_addtimerskill(src, tick + 3000, bl->id, src->x, src->y, skill_id, skill_lv, 0, flag);

	if (md)
	{
		// custom hack to make the mob display the skill, because these skills don't show the skill use text themselves
		// NOTE: mobs don't have the sprite animation that is used when performing this skill (will cause glitches)
		char temp[70];
		snprintf(temp, sizeof(temp), "%s : %s !!", md->name, skill_get_desc(skill_id));
		clif_disp_overhead(md, temp);
	}
	break;

case BA_PANGVOICE:
#ifdef RENEWAL
	// In Renewal it causes Confusion and Bleeding to 100% base chance
	sc_start(src, bl, SC_CONFUSION, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	sc_start(src, bl, SC_BLEEDING, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
#else
	// In Pre-renewal it causes Confusion to 70% base chance
	sc_start(src, bl, SC_CONFUSION, 70, skill_lv, skill_get_time(skill_id, skill_lv));
#endif
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case DC_WINKCHARM:
	if (dstsd)
	{
#ifdef RENEWAL
		// In Renewal it causes Confusion and Hallucination to 100% base chance
		sc_start(src, bl, SC_CONFUSION, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		sc_start(src, bl, SC_HALLUCINATION, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
#else
		// In Pre-Renewal it only causes Wink Charm, if Confusion was successfully started
		if (sc_start(src, bl, SC_CONFUSION, 10, skill_lv, skill_get_time(skill_id, skill_lv)))
			sc_start(src, bl, type, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
#endif
	}
	else if (dstmd)
	{
		// For monsters it causes Wink Charm with a chance depending on the level difference
		if (sc_start2(src, bl, type, (status_get_lv(src) - status_get_lv(bl)) + 40, skill_lv, src->id, skill_get_time2(skill_id, skill_lv)))
		{
			// This triggers a 0 damage event and might make the monster switch target to caster
			battle_damage(src, bl, 0, 1, skill_lv, 0, ATK_DEF, BF_WEAPON | BF_LONG | BF_NORMAL, true, tick, false);
		}
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

#ifdef RENEWAL
case BD_LULLABY:
case BD_RICHMANKIM:
case BD_ETERNALCHAOS:
case BD_DRUMBATTLEFIELD:
case BD_RINGNIBELUNGEN:
case BD_ROKISWEIL:
case BD_INTOABYSS:
case BD_SIEGFRIED:
case BA_DISSONANCE:
case BA_POEMBRAGI:
case BA_WHISTLE:
case BA_ASSASSINCROSS:
case BA_APPLEIDUN:
case DC_UGLYDANCE:
case DC_HUMMING:
case DC_DONTFORGETME:
case DC_FORTUNEKISS:
case DC_SERVICEFORYOU:
	skill_castend_song(src, skill_id, skill_lv, tick);
	break;
#endif

case TF_STEAL:
	if (sd)
	{
		if (pc_steal_item(sd, bl, skill_lv))
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		else
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
	}
	break;

case RG_STEALCOIN:
	if (sd)
	{
		if (pc_steal_coin(sd, bl))
		{
			dstmd->state.provoke_flag = src->id;
			mob_target(dstmd, src, skill_get_range2(src, skill_id, skill_lv, true));
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		}
		else
			clif_skill_fail(*sd, skill_id);
	}
	break;

case MG_STONECURSE:
{
	if (status_has_mode(tstatus, MD_STATUSIMMUNE))
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		break;
	}
	if (status_isimmune(bl) || !tsc)
		break;

	int32 brate = 0;

	if (sd && sd->sc.getSCE(SC_PETROLOGY_OPTION))
		brate = sd->sc.getSCE(SC_PETROLOGY_OPTION)->val3;

	// Except for players, the skill animation shows even if the status change doesn't start
	// Players get a skill has failed message instead
	if (sc_start2(src, bl, type, (skill_lv * 4 + 20) + brate, skill_lv, src->id, skill_get_time2(skill_id, skill_lv), skill_get_time(skill_id, skill_lv)) || sd == nullptr)
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	else
	{
		clif_skill_fail(*sd, skill_id);
		// Level 6-10 doesn't consume a red gem if it fails [celest]
		if (skill_lv > 5)
		{ // not to consume items
			map_freeblock_unlock();
			return 0;
		}
	}
}
break;

case NV_FIRSTAID:
	clif_skill_nodamage(src, *bl, skill_id, 5);
	status_heal(bl, 5, 0, 0);
	break;

case AL_CURE:
	if (status_isimmune(bl))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, false);
		break;
	}
	status_change_end(bl, SC_SILENCE);
	status_change_end(bl, SC_BLIND);
	status_change_end(bl, SC_CONFUSION);
	status_change_end(bl, SC_BITESCAR);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case TF_DETOXIFY:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_change_end(bl, SC_POISON);
	status_change_end(bl, SC_DPOISON);
	break;

case PR_STRECOVERY:
	if (status_isimmune(bl))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, false);
		break;
	}
	if (battle_check_undead(tstatus->race, tstatus->def_ele))
		skill_addtimerskill(src, tick + 1000, bl->id, 0, 0, skill_id, skill_lv, 100, flag);
	else
	{
		// Bodystate is reset to "normal" for non-undead
		if (tsc)
		{
			// The following are bodystate status changes
			status_change_end(bl, SC_STONE);
			status_change_end(bl, SC_FREEZE);
			status_change_end(bl, SC_STUN);
			status_change_end(bl, SC_SLEEP);
			status_change_end(bl, SC_STONEWAIT);
			status_change_end(bl, SC_BURNING);
			status_change_end(bl, SC_WHITEIMPRISON);
		}
		// Resetting bodystate to normal always also resets the monster AI to idle
		if (dstmd)
			mob_unlocktarget(dstmd, tick);
	}
	if (tsc)
	{
		// Ends SC_NETHERWORLD and SC_NORECOVER_STATE (even on undead)
		status_change_end(bl, SC_NETHERWORLD);
		status_change_end(bl, SC_NORECOVER_STATE);
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

// Mercenary Supportive Skills
case MER_BENEDICTION:
	status_change_end(bl, SC_CURSE);
	status_change_end(bl, SC_BLIND);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case MER_COMPRESS:
	status_change_end(bl, SC_BLEEDING);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case MER_MENTALCURE:
	status_change_end(bl, SC_CONFUSION);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case MER_RECUPERATE:
	status_change_end(bl, SC_POISON);
	status_change_end(bl, SC_DPOISON);
	status_change_end(bl, SC_SILENCE);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case MER_REGAIN:
	status_change_end(bl, SC_SLEEP);
	status_change_end(bl, SC_STUN);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case MER_TENDER:
	status_change_end(bl, SC_FREEZE);
	status_change_end(bl, SC_STONE);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case MER_SCAPEGOAT:
	if (mer && mer->master)
	{
		status_heal(mer->master, mer->battle_status.hp, 0, 2);
		status_damage(src, src, mer->battle_status.max_hp, 0, 0, 1, skill_id);
	}
	break;

case MER_ESTIMATION:
	if (!mer)
		break;
	sd = mer->master;
	[[fallthrough]];
case WZ_ESTIMATION:
	if (sd == nullptr)
		break;
	if (dstsd)
	{ // Fail on Players
		clif_skill_fail(*sd, skill_id);
		break;
	}

	if (dstmd != nullptr)
		clif_skill_estimation(*sd, *dstmd);
	if (skill_id == MER_ESTIMATION)
		sd = nullptr;
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case BS_REPAIRWEAPON:
	if (sd && dstsd)
		clif_item_repair_list(*sd, *dstsd, skill_lv);
	break;

case MC_IDENTIFY:
	if (sd)
	{
		clif_item_identify_list(sd);
		if (sd->menuskill_id != MC_IDENTIFY)
		{ // failed, dont consume anything
			map_freeblock_unlock();
			return 1;
		}
		else
		{ // consume sp only if succeeded
			struct s_skill_condition req = skill_get_requirement(sd, skill_id, skill_lv);
			status_zap(src, 0, req.sp);
		}
	}
	break;

// Weapon Refining [Celest]
case WS_WEAPONREFINE:
	if (sd != nullptr)
	{
		clif_item_refine_list(*sd);
	}
	break;

case MC_VENDING:
	if (sd)
	{ // Prevent vending of GMs with unnecessary Level to trade/drop. [Skotlex]
		if (!pc_can_give_items(sd))
			clif_skill_fail(*sd, skill_id);
		else
		{
			sd->state.prevend = 1;
			sd->state.workinprogress = WIP_DISABLE_ALL;
			sd->vend_skill_lv = skill_lv;
			ARR_FIND(0, MAX_CART, i, sd->cart.u.items_cart[i].nameid && sd->cart.u.items_cart[i].id == 0);
			if (i < MAX_CART)
			{
				// Save the cart before opening the vending UI
				sd->state.pending_vending_ui = true;
				intif_storage_save(sd, &sd->cart);
			}
			else
			{
				// Instantly open the vending UI
				sd->state.pending_vending_ui = false;
				clif_openvendingreq(*sd, 2 + skill_lv);
			}
		}
	}
	break;

case AL_TELEPORT:
case ALL_ODINS_RECALL:
	if (sd != nullptr)
	{
		if (map_getmapflag(bl->m, MF_NOTELEPORT) && skill_lv <= 2)
		{
			clif_skill_teleportmessage(*sd, NOTIFY_MAPINFO_CANT_TP);
			break;
		}
		if (!battle_config.duel_allow_teleport && sd->duel_group && skill_lv <= 2)
		{ // duel restriction [LuzZza]
			char output[128];
			sprintf(output, msg_txt(sd, 365), skill_get_name(AL_TELEPORT));
			clif_displaymessage(sd->fd, output); //"Duel: Can't use %s in duel."
			break;
		}

		if (sd->state.autocast || ((sd->skillitem == AL_TELEPORT || battle_config.skip_teleport_lv1_menu) && skill_lv == 1) || skill_lv == 3)
		{
			if (skill_lv == 1)
				pc_randomwarp(sd, CLR_TELEPORT);
			else
				pc_setpos(sd, mapindex_name2id(sd->status.save_point.map), sd->status.save_point.x, sd->status.save_point.y, CLR_TELEPORT);
			break;
		}

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);

		std::vector<std::string> maps = {
			"Random"};

		if (skill_lv == 1 && skill_id != ALL_ODINS_RECALL)
		{
			clif_skill_warppoint(*sd, skill_id, skill_lv, maps);
		}
		else
		{
			maps.push_back(sd->status.save_point.map);

			clif_skill_warppoint(*sd, skill_id, skill_lv, maps);
		}
	}
	else
		unit_warp(bl, -1, -1, -1, CLR_TELEPORT);
	break;

case NPC_EXPULSION:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	unit_warp(bl, -1, -1, -1, CLR_TELEPORT);
	break;

case AL_HOLYWATER:
	if (sd)
	{
		if (skill_produce_mix(sd, skill_id, ITEMID_HOLY_WATER, 0, 0, 0, 1, -1))
		{
			struct skill_unit *su;
			if ((su = map_find_skill_unit_oncell(bl, bl->x, bl->y, NJ_SUITON, nullptr, 0)) != nullptr)
				skill_delunit(su);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		}
		else
			clif_skill_fail(*sd, skill_id);
	}
	break;

case TF_PICKSTONE:
	if (sd)
	{
		unsigned char eflag;
		struct item item_tmp;
		struct block_list tbl;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		memset(&item_tmp, 0, sizeof(item_tmp));
		memset(&tbl, 0, sizeof(tbl)); // [MouseJstr]
		item_tmp.nameid = ITEMID_STONE;
		item_tmp.identify = 1;
		tbl.id = 0;
		// Commented because of duplicate animation [Lemongrass]
		// At the moment this displays the pickup animation a second time
		// If this is required in older clients, we need to add a version check here
		// clif_takeitem(*sd,tbl);
		eflag = pc_additem(sd, &item_tmp, 1, LOG_TYPE_PRODUCE);
		if (eflag)
		{
			clif_additem(sd, 0, 0, eflag);
			if (battle_config.skill_drop_items_full)
				map_addflooritem(&item_tmp, 1, sd->m, sd->x, sd->y, 0, 0, 0, 4, 0);
		}
	}
	break;
case ASC_CDP:
	if (sd)
	{
		if (skill_produce_mix(sd, skill_id, ITEMID_POISON_BOTTLE, 0, 0, 0, 1, -1)) // Produce a Poison Bottle.
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		else
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL_STUFF_INSUFFICIENT);
	}
	break;

case RG_STRIPWEAPON:
case RG_STRIPSHIELD:
case RG_STRIPARMOR:
case RG_STRIPHELM:
case ST_FULLSTRIP:
case GC_WEAPONCRUSH:
case SC_STRIPACCESSARY:
case ABC_STRIP_SHADOW:
{
	bool i;

	// Special message when trying to use strip on FCP [Jobbie]
	if (sd && skill_id == ST_FULLSTRIP && tsc && tsc->getSCE(SC_CP_WEAPON) && tsc->getSCE(SC_CP_HELM) && tsc->getSCE(SC_CP_ARMOR) && tsc->getSCE(SC_CP_SHIELD))
	{
		clif_gospel_info(*sd, 0x28);
		break;
	}

	if ((i = skill_strip_equip(src, bl, skill_id, skill_lv)) || (skill_id != ST_FULLSTRIP && skill_id != GC_WEAPONCRUSH))
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, i);

	// Nothing stripped.
	if (sd && !i)
		clif_skill_fail(*sd, skill_id);
	break;
}

case AM_BERSERKPITCHER:
case AM_POTIONPITCHER:
{
	int32 j, hp = 0, sp = 0;
	if (dstmd && dstmd->mob_id == MOBID_EMPERIUM)
	{
		map_freeblock_unlock();
		return 1;
	}
	if (sd)
	{
		int32 x, bonus = 100;
		struct s_skill_condition require = skill_get_requirement(sd, skill_id, skill_lv);
		x = skill_lv % 11 - 1;
		j = pc_search_inventory(sd, require.itemid[x]);
		if (j < 0 || require.itemid[x] <= 0)
		{
			clif_skill_fail(*sd, skill_id);
			map_freeblock_unlock();
			return 1;
		}
		if (sd->inventory_data[j] == nullptr || sd->inventory.u.items_inventory[j].amount < require.amount[x])
		{
			clif_skill_fail(*sd, skill_id);
			map_freeblock_unlock();
			return 1;
		}
		if (skill_id == AM_BERSERKPITCHER)
		{
			if (dstsd && dstsd->status.base_level < (uint32)sd->inventory_data[j]->elv)
			{
				clif_skill_fail(*sd, skill_id);
				map_freeblock_unlock();
				return 1;
			}
		}
		potion_flag = 1;
		potion_hp = potion_sp = potion_per_hp = potion_per_sp = 0;
		potion_target = bl->id;
		run_script(sd->inventory_data[j]->script, 0, sd->id, 0);
		potion_flag = potion_target = 0;
		if (sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_ALCHEMIST)
			bonus += sd->status.base_level;
		if (potion_per_hp > 0 || potion_per_sp > 0)
		{
			hp = tstatus->max_hp * potion_per_hp / 100;
			hp = hp * (100 + pc_checkskill(sd, AM_POTIONPITCHER) * 10 + pc_checkskill(sd, AM_LEARNINGPOTION) * 5) * bonus / 10000;
			if (dstsd)
			{
				sp = dstsd->status.max_sp * potion_per_sp / 100;
				sp = sp * (100 + pc_checkskill(sd, AM_POTIONPITCHER) * 10 + pc_checkskill(sd, AM_LEARNINGPOTION) * 5) * bonus / 10000;
			}
		}
		else
		{
			if (potion_hp > 0)
			{
				hp = potion_hp * (100 + pc_checkskill(sd, AM_POTIONPITCHER) * 10 + pc_checkskill(sd, AM_LEARNINGPOTION) * 5) * bonus / 10000;
				hp = hp * (100 + (tstatus->vit * 2)) / 100;
				if (dstsd)
					hp = hp * (100 + pc_checkskill(dstsd, SM_RECOVERY) * 10) / 100;
			}
			if (potion_sp > 0)
			{
				sp = potion_sp * (100 + pc_checkskill(sd, AM_POTIONPITCHER) * 10 + pc_checkskill(sd, AM_LEARNINGPOTION) * 5) * bonus / 10000;
				sp = sp * (100 + (tstatus->int_ * 2)) / 100;
				if (dstsd)
					sp = sp * (100 + pc_checkskill(dstsd, MG_SRECOVERY) * 10) / 100;
			}
		}

		if ((bonus = pc_get_itemgroup_bonus_group(sd, IG_POTION, sd->itemgrouphealrate)))
		{
			hp += hp * bonus / 100;
		}

		if ((bonus = pc_get_itemgroup_bonus_group(sd, IG_POTION, sd->itemgroupsphealrate)))
		{
			sp += sp * bonus / 100;
		}

		if ((j = pc_skillheal_bonus(sd, skill_id)))
		{
			hp += hp * j / 100;
			sp += sp * j / 100;
		}
	}
	else
	{
		// Maybe replace with potion_hp, but I'm unsure how that works [Playtester]
		switch (skill_lv)
		{
		case 1:
			hp = 45;
			break;
		case 2:
			hp = 105;
			break;
		case 3:
			hp = 175;
			break;
		default:
			hp = 325;
			break;
		}
		hp = (hp + rnd() % (skill_lv * 20 + 1)) * (150 + skill_lv * 10) / 100;
		hp = hp * (100 + (tstatus->vit * 2)) / 100;
		if (dstsd)
			hp = hp * (100 + pc_checkskill(dstsd, SM_RECOVERY) * 10) / 100;
	}
	if (dstsd && (j = pc_skillheal2_bonus(dstsd, skill_id)))
	{
		hp += hp * j / 100;
		sp += sp * j / 100;
	}
	// Final heal increased by HPlus.
	// Is this the right place for this??? [Rytech]
	// Can HPlus also affect SP recovery???
	if (sd && sstatus->hplus > 0)
	{
		hp += hp * sstatus->hplus / 100;
		sp += sp * sstatus->hplus / 100;
	}
	if (tsc != nullptr && !tsc->empty())
	{
		uint8 penalty = 0;

		if (tsc->getSCE(SC_WATER_INSIGNIA) && tsc->getSCE(SC_WATER_INSIGNIA)->val1 == 2)
		{
			hp += hp / 10;
			sp += sp / 10;
		}
		if (tsc->getSCE(SC_CRITICALWOUND))
			penalty += tsc->getSCE(SC_CRITICALWOUND)->val2;
		if (tsc->getSCE(SC_DEATHHURT) && tsc->getSCE(SC_DEATHHURT)->val3)
			penalty += 20;
		if (tsc->getSCE(SC_NORECOVER_STATE))
			penalty = 100;
		if (penalty > 0)
		{
			hp -= hp * penalty / 100;
			sp -= sp * penalty / 100;
		}
	}

#ifdef RENEWAL
	if (bl->type == BL_HOM)
		hp *= 3; // Heal effectiveness is 3x for Homunculus
#endif

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (hp > 0 || (skill_id == AM_POTIONPITCHER && sp <= 0))
		clif_skill_nodamage(nullptr, *bl, AL_HEAL, hp, 1);
	if (sp > 0)
		clif_skill_nodamage(nullptr, *bl, MG_SRECOVERY, sp);
	if (tsc)
	{
#ifdef RENEWAL
		if (tsc->getSCE(SC_EXTREMITYFIST))
			sp = 0;
#endif
		if (tsc->getSCE(SC_NORECOVER_STATE))
		{
			hp = 0;
			sp = 0;
		}
	}
	status_heal(bl, hp, sp, 0);
}
break;
case AM_CP_WEAPON:
case AM_CP_SHIELD:
case AM_CP_ARMOR:
case AM_CP_HELM:
{
	uint32 equip[] = {EQP_WEAPON, EQP_SHIELD, EQP_ARMOR, EQP_HEAD_TOP};

	if (sd && (bl->type != BL_PC || (dstsd && pc_checkequip(dstsd, equip[skill_id - AM_CP_WEAPON]) < 0)))
	{
		clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock(); // Don't consume item requirements
		return 0;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
}
break;
case AM_TWILIGHT1:
	if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		// Prepare 200 White Potions.
		if (!skill_produce_mix(sd, skill_id, ITEMID_WHITE_POTION, 0, 0, 0, 200, -1))
			clif_skill_fail(*sd, skill_id);
	}
	break;
case AM_TWILIGHT2:
	if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		// Prepare 200 Slim White Potions.
		if (!skill_produce_mix(sd, skill_id, ITEMID_WHITE_SLIM_POTION, 0, 0, 0, 200, -1))
			clif_skill_fail(*sd, skill_id);
	}
	break;
case AM_TWILIGHT3:
	if (sd)
	{
		int32 ebottle = pc_search_inventory(sd, ITEMID_EMPTY_BOTTLE);
		int16 alcohol_idx = -1, acid_idx = -1, fire_idx = -1;
		if (ebottle >= 0)
			ebottle = sd->inventory.u.items_inventory[ebottle].amount;
		// check if you can produce all three, if not, then fail:
		if (!(alcohol_idx = skill_can_produce_mix(sd, ITEMID_ALCOHOL, -1, 100))	   // 100 Alcohol
			|| !(acid_idx = skill_can_produce_mix(sd, ITEMID_ACID_BOTTLE, -1, 50)) // 50 Acid Bottle
			|| !(fire_idx = skill_can_produce_mix(sd, ITEMID_FIRE_BOTTLE, -1, 50)) // 50 Flame Bottle
			|| ebottle < 200													   // 200 empty bottle are required at total.
		)
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		skill_produce_mix(sd, skill_id, ITEMID_ALCOHOL, 0, 0, 0, 100, alcohol_idx - 1);
		skill_produce_mix(sd, skill_id, ITEMID_ACID_BOTTLE, 0, 0, 0, 50, acid_idx - 1);
		skill_produce_mix(sd, skill_id, ITEMID_FIRE_BOTTLE, 0, 0, 0, 50, fire_idx - 1);
	}
	break;
case SA_DISPELL:
	if (flag & 1 || (i = skill_get_splash(skill_id, skill_lv)) < 1)
	{
		if (sd && dstsd && !map_flag_vs(sd->m) && (!sd->duel_group || sd->duel_group != dstsd->duel_group) && (!sd->status.party_id || sd->status.party_id != dstsd->status.party_id))
			break; // Outside PvP it should only affect party members and no skill fail message
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		if ((dstsd && (dstsd->class_ & MAPID_UPPERMASK) == MAPID_SOUL_LINKER) || (tsc && tsc->getSCE(SC_SPIRIT) && tsc->getSCE(SC_SPIRIT)->val2 == SL_ROGUE) // Rogue's spirit defends againt32 dispel.
			|| rnd() % 100 >= 50 + 10 * skill_lv)
		{
			if (sd)
				clif_skill_fail(*sd, skill_id);
			break;
		}
		if (status_isimmune(bl))
			break;

		// Remove bonus_script by Dispell
		if (dstsd)
			pc_bonus_script_clear(dstsd, BSF_REM_ON_DISPELL);
		// Monsters will unlock their target instead
		else if (dstmd)
			mob_unlocktarget(dstmd, tick);

		if (tsc == nullptr || tsc->empty())
			break;

		// Statuses that can't be Dispelled
		for (const auto &it : status_db)
		{
			sc_type status = static_cast<sc_type>(it.first);

			if (!tsc->getSCE(status))
				continue;

			if (it.second->flag[SCF_NODISPELL])
				continue;
			switch (status)
			{
			// bugreport:4888 these songs may only be dispelled if you're not in their song area anymore
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
			if (i == SC_BERSERK || i == SC_SATURDAYNIGHTFEVER)
				tsc->getSCE(status)->val2 = 0; // Mark a dispelled berserk to avoid setting hp to 100 by setting hp penalty to 0.
			status_change_end(bl, status);
		}
		break;
	}

	// Affect all targets on splash area.
	map_foreachinallrange(skill_area_sub, bl, i, BL_CHAR,
						  src, skill_id, skill_lv, tick, flag | 1,
						  skill_castend_damage_id);
	break;

case TF_BACKSLIDING: // This is the correct implementation as per packet logging information. [Skotlex]
{
	// Backsliding makes you immune to being stopped for 200ms, but only if you don't have the endure effect yet
	if (unit_data *ud = unit_bl2ud(bl); ud != nullptr && !status_isendure(*bl, tick, true))
		ud->endure_tick = tick + 200;

	int16 blew_count = skill_blown(src, bl, skill_get_blewcount(skill_id, skill_lv), unit_getdir(bl), (enum e_skill_blown)(BLOWN_IGNORE_NO_KNOCKBACK
#ifdef RENEWAL
																														   | BLOWN_DONT_SEND_PACKET
#endif
																														   ));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
#ifdef RENEWAL
	if (blew_count > 0)
		clif_blown(src); // Always blow, otherwise it shows a casting animation. [Lemongrass]
#else
	clif_slide(*bl, bl->x, bl->y); // Show the casting animation on pre-re
#endif
}
break;

case TK_HIGHJUMP:
{
	int32 x, y, dir = unit_getdir(src);
	struct map_data *mapdata = &map[src->m];

	// Fails on noteleport maps, except for GvG and BG maps [Skotlex]
	if (mapdata->getMapFlag(MF_NOTELEPORT) &&
		!(mapdata->getMapFlag(MF_BATTLEGROUND) || mapdata_flag_gvg2(mapdata)))
	{
		clif_skill_nodamage(src, *bl, TK_HIGHJUMP, skill_lv);
		break;
	}
	else if (dir % 2)
	{
		// Diagonal
		x = src->x + dirx[dir] * (skill_lv * 4) / 3;
		y = src->y + diry[dir] * (skill_lv * 4) / 3;
	}
	else
	{
		x = src->x + dirx[dir] * skill_lv * 2;
		y = src->y + diry[dir] * skill_lv * 2;
	}

	int32 x1 = x + dirx[dir];
	int32 y1 = y + diry[dir];

	clif_skill_nodamage(src, *bl, TK_HIGHJUMP, skill_lv);
	if (!map_count_oncell(src->m, x, y, BL_PC | BL_NPC | BL_MOB, 0) && map_getcell(src->m, x, y, CELL_CHKREACH) &&
		!map_count_oncell(src->m, x1, y1, BL_PC | BL_NPC | BL_MOB, 0) && map_getcell(src->m, x1, y1, CELL_CHKREACH) &&
		unit_movepos(src, x, y, 1, 0))
		clif_blown(src);
}
break;

case SA_CASTCANCEL:
case SO_SPELLFIST:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	unit_skillcastcancel(src, 1);
	if (sd)
	{
		int32 sp = skill_get_sp(sd->skill_id_old, sd->skill_lv_old);
		if (skill_id == SO_SPELLFIST)
		{
			sc_start4(src, src, type, 100, skill_lv, sd->skill_id_old, sd->skill_lv_old, 0, skill_get_time(skill_id, skill_lv));
			sd->skill_id_old = sd->skill_lv_old = 0;
			break;
		}
		sp = sp * (90 - (skill_lv - 1) * 20) / 100;
		if (sp < 0)
			sp = 0;
		status_zap(src, 0, sp);
	}
	break;
case SA_SPELLBREAKER:
{
	int32 sp;
	if (dstsd && tsc && tsc->getSCE(SC_MAGICROD))
	{
		// If target enemy player has Magic Rod, then 20% of your SP is transferred to that player
		sp = status_percent_damage(bl, src, 0, -20, false);
		status_heal(bl, 0, sp, 2);
	}
	else
	{
		struct unit_data *ud = unit_bl2ud(bl);
		if (!ud || ud->skilltimer == INVALID_TIMER)
			break; // Nothing to cancel.
		int32 hp = 0;
		if (status_has_mode(tstatus, MD_STATUSIMMUNE))
		{ // Only 10% success chance against status immune. [Skotlex]
			if (rnd_chance(90, 100))
			{
				if (sd)
					clif_skill_fail(*sd, skill_id);
				break;
			}
		}
#ifdef RENEWAL
		else // HP damage does not work on bosses in renewal
#endif
			if (skill_lv >= 5 && (!dstsd || map_flag_vs(bl->m))) // HP damage only on pvp-maps when against players.
			hp = tstatus->max_hp / 50;							 // Siphon 2% HP at level 5

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		unit_skillcastcancel(bl, 0);
		sp = skill_get_sp(ud->skill_id, ud->skill_lv);
		status_zap(bl, 0, sp);
		// Recover some of the SP used
		status_heal(src, 0, sp * (25 * (skill_lv - 1)) / 100, 2);

		// If damage would be lethal, it does not deal damage
		if (hp && hp < tstatus->hp)
		{
			clif_damage(*src, *bl, tick, 0, 0, hp, 0, DMG_NORMAL, 0, false);
			status_zap(bl, hp, 0);
			// Recover 50% of damage dealt
			status_heal(src, hp / 2, 0, 2);
		}
	}
}
break;
case SA_MAGICROD:
#ifdef RENEWAL
	clif_skill_nodamage(src, *src, SA_MAGICROD, skill_lv);
#endif
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case SA_AUTOSPELL:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd)
	{
		sd->state.workinprogress = WIP_DISABLE_ALL;
		clif_autospell(*sd, skill_lv);
	}
	else
	{
		int32 maxlv = 1, spellid = 0;
		static const int32 spellarray[3] = {MG_COLDBOLT, MG_FIREBOLT, MG_LIGHTNINGBOLT};

		if (skill_lv >= 10)
		{
			spellid = MG_FROSTDIVER;
			//				if (tsc && tsc->getSCE(SC_SPIRIT) && tsc->getSCE(SC_SPIRIT)->val2 == SA_SAGE)
			//					maxlv = 10;
			//				else
			maxlv = skill_lv - 9;
		}
		else if (skill_lv >= 8)
		{
			spellid = MG_FIREBALL;
			maxlv = skill_lv - 7;
		}
		else if (skill_lv >= 5)
		{
			spellid = MG_SOULSTRIKE;
			maxlv = skill_lv - 4;
		}
		else if (skill_lv >= 2)
		{
			int32 i_rnd = rnd() % 3;
			spellid = spellarray[i_rnd];
			maxlv = skill_lv - 1;
		}
		else if (skill_lv > 0)
		{
			spellid = MG_NAPALMBEAT;
			maxlv = 3;
		}

		if (spellid > 0)
			sc_start4(src, src, SC_AUTOSPELL, 100, skill_lv, spellid, maxlv, 0,
					  skill_get_time(SA_AUTOSPELL, skill_lv));
	}
	break;

case BS_GREED:
	if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_greed, bl,
							  skill_get_splash(skill_id, skill_lv), BL_ITEM, bl);
	}
	break;

case SA_ELEMENTWATER:
case SA_ELEMENTFIRE:
case SA_ELEMENTGROUND:
case SA_ELEMENTWIND:
	if (sd && (!dstmd || status_has_mode(tstatus, MD_STATUSIMMUNE))) // Only works on monsters (Except status immune monsters).
		break;
	[[fallthrough]];
case NPC_ATTRICHANGE:
case NPC_CHANGEWATER:
case NPC_CHANGEGROUND:
case NPC_CHANGEFIRE:
case NPC_CHANGEWIND:
case NPC_CHANGEPOISON:
case NPC_CHANGEHOLY:
case NPC_CHANGEDARKNESS:
case NPC_CHANGETELEKINESIS:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start2(src, bl, type, 100, skill_lv, skill_get_ele(skill_id, skill_lv),
								  skill_get_time(skill_id, skill_lv)));
	break;

case NPC_PROVOCATION:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (md)
		mob_unlocktarget(md, tick);
	break;

case NPC_REBIRTH:
	if (md && md->state.rebirth)
		break; // only works once
	sc_start(src, bl, type, 100, skill_lv, INFINITE_TICK);
	break;

case NPC_DARKBLESSING:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start2(src, bl, type, (50 + skill_lv * 5), skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv)));
	break;

case NPC_LICK:
	status_zap(bl, 0, 100);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, (skill_lv * 20), skill_lv, skill_get_time2(skill_id, skill_lv)));
	break;

case NPC_SUICIDE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_kill(src); // When suiciding, neither exp nor drops is given.
	break;

case NPC_SUMMONSLAVE:
case NPC_SUMMONMONSTER:
case NPC_DEATHSUMMON:
	if (md && md->skill_idx >= 0)
		mob_summonslave(md, md->db->skill[md->skill_idx]->val, skill_lv, skill_id);
	break;

case NPC_CALLSLAVE:
	mob_warpslave(src, MOB_SLAVEDISTANCE);
	break;

case NPC_RANDOMMOVE:
	if (md != nullptr)
	{
		// This skill creates fake casting state where a monster moves while showing a cast bar
		int32 tricktime = MOB_SKILL_INTERVAL * 3;
		md->trickcasting = tick + tricktime;
		clif_skillcasting(*src, src, 0, 0, skill_id, skill_lv, ELE_FIRE, tricktime + MOB_SKILL_INTERVAL / 2);
		// Monster cannot be stopped while moving
		md->state.can_escape = 1;
		// Move up to 8 cells
		unit_escape(md, bl, 8, 3);
	}
	break;

case NPC_SPEEDUP:
	if (md != nullptr)
	{
		// Officially, trickcasting continues as long as there are more than 700ms left
		int32 trickstop = (MOB_SKILL_INTERVAL * 7) / 10;
		if (DIFF_TICK(md->trickcasting, tick) >= trickstop)
		{
			// This skill directly modifies a monster's base speed value
			md->base_status->speed = std::max(md->base_status->speed - 250, MIN_WALK_SPEED);
			// Need to recalc speed based on new base value
			status_calc_bl(md, {SCB_SPEED});
			// We use skills only on each full cell, to fix the inaccuracy we do this on last move interval
			if (DIFF_TICK(md->trickcasting, tick) < trickstop + MOB_SKILL_INTERVAL)
				md->last_skillcheck = tick + 100;
		}
		else
		{
			// Synchronize skill usage
			md->last_skillcheck = md->trickcasting;
			// Causes monster to stop and get ready for next alchemist skill
			md->trickcasting = 0;
			md->state.can_escape = 0;
		}
	}
	break;

case NPC_REVENGE:
	// not really needed... but adding here anyway ^^
	if (md && md->master_id > 0)
	{
		struct block_list *mbl, *tbl;
		if ((mbl = map_id2bl(md->master_id)) == nullptr ||
			(tbl = battle_gettargeted(mbl)) == nullptr)
			break;
		md->state.provoke_flag = tbl->id;
		mob_target(md, tbl, sstatus->rhw.range);
	}
	break;

case NPC_RUN:
	if (md)
	{
		block_list *tbl = map_id2bl(md->target_id);

		if (tbl)
		{
			md->state.can_escape = 1;
			mob_unlocktarget(md, tick);
			// Official distance is 7, if level > 1, distance = level
			t_tick time = unit_escape(src, tbl, skill_lv > 1 ? skill_lv : 7, 3);

			if (time)
			{
				// Need to set state here as it's not set otherwise
				mob_setstate(*md, MSS_WALK);
				// Set AI to inactive for the duration of this movement
				md->next_thinktime = tick + time;
			}
		}
	}
	break;

case NPC_TRANSFORMATION:
case NPC_METAMORPHOSIS:
	if (md && md->skill_idx >= 0)
	{
		int32 class_ = mob_random_class(md->db->skill[md->skill_idx]->val, 0);
		if (skill_lv > 1) // Multiply the rest of mobs. [Skotlex]
			mob_summonslave(md, md->db->skill[md->skill_idx]->val, skill_lv - 1, skill_id);
		if (class_)
			mob_class_change(md, class_);
	}
	break;

case NPC_EMOTION_ON:
case NPC_EMOTION:
	// val[0] is the emotion to use.
	// NPC_EMOTION & NPC_EMOTION_ON can change a mob's mode 'permanently' [Skotlex]
	// val[1] 'sets' the mode
	// val[2] adds to the current mode
	// val[3] removes from the current mode
	// val[4] if set, asks to delete the previous mode change.
	if (md && md->skill_idx >= 0 && tsc)
	{
		clif_emotion(*bl, static_cast<emotion_type>(md->db->skill[md->skill_idx]->val[0]));
		if (md->db->skill[md->skill_idx]->val[4] && tsce)
			status_change_end(bl, type);

		// If mode gets set by NPC_EMOTION then the target should be reset [Playtester]
		if (!battle_config.npc_emotion_behavior && skill_id == NPC_EMOTION && md->state.skillstate != MSS_IDLE && md->state.skillstate != MSS_WALK && md->db->skill[md->skill_idx]->val[1])
			mob_unlocktarget(md, tick);

		if (md->db->skill[md->skill_idx]->val[1] || md->db->skill[md->skill_idx]->val[2])
			sc_start4(src, src, type, 100, skill_lv,
					  md->db->skill[md->skill_idx]->val[1],
					  md->db->skill[md->skill_idx]->val[2],
					  md->db->skill[md->skill_idx]->val[3],
					  skill_get_time(skill_id, skill_lv));

		// Reset aggressive state depending on resulting mode
		if (!battle_config.npc_emotion_behavior)
			md->state.aggressive = status_has_mode(&md->status, MD_ANGRY) ? 1 : 0;
	}
	break;

case NPC_POWERUP:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start2(src, bl, type, 100, 200, 100, skill_get_time(skill_id, skill_lv)));
	break;

case NPC_AGIUP:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start2(src, bl, type, 100, 50, 100, skill_get_time(skill_id, skill_lv)));
	break;

case NPC_INVISIBLE:
	// Have val4 passed as 6 is for "infinite cloak" (do not end on attack/skill use).
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start4(src, bl, type, 100, skill_lv, 0, 0, 6, skill_get_time(skill_id, skill_lv)));
	break;

case NPC_SIEGEMODE:
	// Not implemented/used: Gives EFST_SIEGEMODE which reduces speed to 1000.
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case NPC_INVINCIBLEOFF:
case MER_INVINCIBLEOFF2:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_change_end(bl, SC_INVINCIBLE);
	break;

case WE_MALE:
{
	uint8 hp_rate = abs(skill_get_hp_rate(skill_id, skill_lv));

	if (hp_rate && status_get_hp(src) > status_get_max_hp(src) / hp_rate)
	{
		int32 gain_hp = tstatus->max_hp * hp_rate / 100; // The earned is the same % of the target HP than it costed the caster. [Skotlex]

		clif_skill_nodamage(src, *bl, skill_id, status_heal(bl, gain_hp, 0, 0));
	}
}
break;
case WE_FEMALE:
{
	uint8 sp_rate = abs(skill_get_sp_rate(skill_id, skill_lv));

	if (sp_rate && status_get_sp(src) > status_get_max_sp(src) / sp_rate)
	{
		int32 gain_sp = tstatus->max_sp * sp_rate / 100; // The earned is the same % of the target SP than it costed the caster. [Skotlex]

		clif_skill_nodamage(src, *bl, skill_id, status_heal(bl, 0, gain_sp, 0));
	}
}
break;

// parent-baby skills
case WE_BABY:
	if (sd)
	{
		map_session_data *f_sd = pc_get_father(sd);
		map_session_data *m_sd = pc_get_mother(sd);

		if ((!f_sd && !m_sd)																										// if neither was found
			|| (sd->status.party_id != 0 &&																							// not in same party
				((!f_sd || sd->status.party_id != f_sd->status.party_id) && (!m_sd || sd->status.party_id != m_sd->status.party_id) // if both are online they should all be in same team
				 )) ||
			((!f_sd || !check_distance_bl(sd, f_sd, AREA_SIZE)) // not in same screen
			 && (!m_sd || !check_distance_bl(sd, m_sd, AREA_SIZE))))
		{
			clif_skill_fail(*sd, skill_id);
			map_freeblock_unlock();
			return 0;
		}
		status_change_start(src, bl, SC_STUN, 10000, skill_lv, 0, 0, 0, skill_get_time2(skill_id, skill_lv), SCSTART_NORATEDEF);
		if (f_sd)
			sc_start(src, f_sd, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		if (m_sd)
			sc_start(src, m_sd, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	break;

case WE_CALLALLFAMILY:
	if (sd)
	{
		map_session_data *p_sd = pc_get_partner(sd);
		map_session_data *c_sd = pc_get_child(sd);

		if (!p_sd && !c_sd)
		{ // Fail if no family members are found
			clif_skill_fail(*sd, skill_id);
			map_freeblock_unlock();
			return 1;
		}

		// Partner must be on the same map and in same party
		if (p_sd && !status_isdead(*p_sd) && p_sd->m == sd->m && p_sd->status.party_id == sd->status.party_id)
			pc_setpos(p_sd, map_id2index(sd->m), sd->x, sd->y, CLR_TELEPORT);
		// Child must be on the same map and in same party as the parent casting
		if (c_sd && !status_isdead(*c_sd) && c_sd->m == sd->m && c_sd->status.party_id == sd->status.party_id)
			pc_setpos(c_sd, map_id2index(sd->m), sd->x, sd->y, CLR_TELEPORT);
	}
	break;

case WE_ONEFOREVER:
	if (sd)
	{
		map_session_data *p_sd = pc_get_partner(sd);
		map_session_data *c_sd = pc_get_child(sd);

		if (!p_sd && !c_sd && !dstsd)
		{ // Fail if no family members are found
			clif_skill_fail(*sd, skill_id);
			map_freeblock_unlock();
			return 1;
		}
		if (map_flag_gvg2(bl->m) || map_getmapflag(bl->m, MF_BATTLEGROUND))
		{ // No reviving in WoE grounds!
			clif_skill_fail(*sd, skill_id);
			break;
		}
		if (status_isdead(*bl))
		{
			int32 per = 30, sper = 0;

			if (battle_check_undead(tstatus->race, tstatus->def_ele))
				break;
			if (tsc && tsc->getSCE(SC_HELLPOWER))
				break;
			if (map_getmapflag(bl->m, MF_PVP) && dstsd->pvp_point < 0)
				break;
			if (dstsd->special_state.restart_full_recover)
				per = sper = 100;
			if ((dstsd == p_sd || dstsd == c_sd) && status_revive(bl, per, sper)) // Only family members can be revived
				clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		}
	}
	break;

case WE_CHEERUP:
	if (sd)
	{
		map_session_data *f_sd = pc_get_father(sd);
		map_session_data *m_sd = pc_get_mother(sd);

		if (!f_sd && !m_sd && !dstsd)
		{ // Fail if no family members are found
			clif_skill_fail(*sd, skill_id);
			map_freeblock_unlock();
			return 1;
		}
		if (flag & 1)
		{ // Buff can only be given to parents in 7x7 AoE around baby
			if (dstsd == f_sd || dstsd == m_sd)
				clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
		}
		else
			map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_PC, src, skill_id, skill_lv, tick, flag | BCT_ALL | 1, skill_castend_nodamage_id);
	}
	break;

case PF_HPCONVERSION:
{
	int32 hp, sp;
	hp = sstatus->max_hp / 10;
	sp = hp * 10 * skill_lv / 100;
	if (!status_charge(src, hp, 0))
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		break;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_heal(bl, 0, sp, 2);
}
break;

case MA_REMOVETRAP:
case HT_REMOVETRAP:
{
	skill_unit *su = BL_CAST(BL_SKILL, bl);
	std::shared_ptr<s_skill_unit_group> sg;
	std::shared_ptr<s_skill_db> skill_group;

	// Mercenaries can remove any trap
	// Players can only remove their own traps or traps on Vs maps.
	if (su && (sg = su->group) && (src->type == BL_MER || sg->src_id == src->id || map_flag_vs(bl->m)) && (skill_group = skill_db.find(sg->skill_id)) && skill_group->inf2[INF2_ISTRAP])
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		if (sd && !(sg->unit_id == UNT_USED_TRAPS || (sg->unit_id == UNT_ANKLESNARE && sg->val2 != 0)))
		{ // prevent picking up expired traps
			if (battle_config.skill_removetrap_type)
			{ // get back all items used to deploy the trap
				for (i = 0; i < MAX_SKILL_ITEM_REQUIRE; i++)
				{
					if (skill_group->require.itemid[i] > 0)
					{
						int32 flag2;
						struct item item_tmp;
						memset(&item_tmp, 0, sizeof(item_tmp));
						item_tmp.nameid = skill_group->require.itemid[i];
						item_tmp.identify = 1;
						item_tmp.amount = skill_group->require.amount[i];
						if (item_tmp.nameid && (flag2 = pc_additem(sd, &item_tmp, item_tmp.amount, LOG_TYPE_OTHER)))
						{
							clif_additem(sd, 0, 0, flag2);
							if (battle_config.skill_drop_items_full)
								map_addflooritem(&item_tmp, item_tmp.amount, sd->m, sd->x, sd->y, 0, 0, 0, 4, 0);
						}
					}
				}
			}
			else
			{ // get back 1 trap
				struct item item_tmp;
				memset(&item_tmp, 0, sizeof(item_tmp));
				item_tmp.nameid = su->group->item_id ? su->group->item_id : ITEMID_TRAP;
				item_tmp.identify = 1;
				if (item_tmp.nameid && (flag = pc_additem(sd, &item_tmp, 1, LOG_TYPE_OTHER)))
				{
					clif_additem(sd, 0, 0, flag);
					if (battle_config.skill_drop_items_full)
						map_addflooritem(&item_tmp, 1, sd->m, sd->x, sd->y, 0, 0, 0, 4, 0);
				}
			}
		}
		skill_delunit(su);
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
}
break;
case HT_SPRINGTRAP:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	{
		struct skill_unit *su = nullptr;
		if ((bl->type == BL_SKILL) && (su = (struct skill_unit *)bl) && (su->group))
		{
			switch (su->group->unit_id)
			{
			case UNT_ANKLESNARE: // ankle snare
				if (su->group->val2 != 0)
					// if it is already trapping something don't spring it,
					// remove trap should be used instead
					break;
				[[fallthrough]];
			case UNT_BLASTMINE:
			case UNT_SKIDTRAP:
			case UNT_LANDMINE:
			case UNT_SHOCKWAVE:
			case UNT_SANDMAN:
			case UNT_FLASHER:
			case UNT_FREEZINGTRAP:
			case UNT_CLAYMORETRAP:
			case UNT_TALKIEBOX:
				su->group->unit_id = UNT_USED_TRAPS;
				clif_changetraplook(bl, UNT_USED_TRAPS);
				su->group->limit = DIFF_TICK(tick + 1500, su->group->tick);
				su->limit = DIFF_TICK(tick + 1500, su->group->tick);
			}
		}
	}
	break;
case BD_ENCORE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd != nullptr)
	{
		unit_skilluse_id(src, src->id, sd->skill_id_dance, sd->skill_lv_dance);
		// Need to remove remembered skill to prevent permanent halving of SP cost
		sd->skill_id_old = 0;
	}
	break;

case TR_RETROSPECTION:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd)
		unit_skilluse_id(src, src->id, sd->skill_id_song, sd->skill_lv_song);
	break;

case AS_SPLASHER:
	if (status_has_mode(tstatus, MD_STATUSIMMUNE)
	// Renewal dropped the 3/4 hp requirement
#ifndef RENEWAL
		|| tstatus->hp > tstatus->max_hp * 3 / 4
#endif
	)
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock();
		return 1;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start4(src, bl, type, 100, skill_lv, skill_id, src->id, skill_get_time(skill_id, skill_lv), 1000));
	break;

case PF_MINDBREAKER:
{
	if (status_has_mode(tstatus, MD_STATUSIMMUNE) || battle_check_undead(tstatus->race, tstatus->def_ele))
	{
		map_freeblock_unlock();
		return 1;
	}

	if (tsce)
	{ // HelloKitty2 (?) explained that this silently fails when target is
		// already inflicted. [Skotlex]
		map_freeblock_unlock();
		return 1;
	}

	// Has a 55% + skill_lv*5% success chance.
	if (!clif_skill_nodamage(src, *bl, skill_id, skill_lv,
							 sc_start(src, bl, type, 55 + 5 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv))))
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock();
		return 0;
	}

	unit_skillcastcancel(bl, 0);

	if (dstmd)
		mob_target(dstmd, src, skill_get_range2(src, skill_id, skill_lv, true));
}
break;

case PF_SOULCHANGE:
{
	uint32 sp1 = 0, sp2 = 0;
	if (dstmd)
	{
		if (dstmd->state.soul_change_flag)
		{
			if (sd)
				clif_skill_fail(*sd, skill_id);
			break;
		}
		dstmd->state.soul_change_flag = 1;
		sp2 = sstatus->max_sp * 3 / 100;
		status_heal(src, 0, sp2, 2);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		break;
	}
	sp1 = sstatus->sp;
	sp2 = tstatus->sp;
#ifdef RENEWAL
	sp1 = sp1 / 2;
	sp2 = sp2 / 2;
	if (tsc && tsc->getSCE(SC_EXTREMITYFIST))
		sp1 = tstatus->sp;
#endif
	if (tsc && tsc->getSCE(SC_NORECOVER_STATE))
		sp1 = tstatus->sp;
	status_set_sp(src, sp2, 3);
	status_set_sp(bl, sp1, 3);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
}
break;

// Slim Pitcher
case CR_SLIMPITCHER:
	// Updated to block Slim Pitcher from working on barricades and guardian stones.
	if (dstmd && (dstmd->mob_id == MOBID_EMPERIUM || status_get_class_(bl) == CLASS_BATTLEFIELD))
		break;
	if (potion_hp || potion_sp)
	{
		int32 hp = potion_hp, sp = potion_sp;
		hp = hp * (100 + (tstatus->vit * 2)) / 100;
		sp = sp * (100 + (tstatus->int_ * 2)) / 100;
		if (dstsd)
		{
			if (hp)
				hp = hp * (100 + pc_checkskill(dstsd, SM_RECOVERY) * 10 + pc_skillheal2_bonus(dstsd, skill_id)) / 100;
			if (sp)
				sp = sp * (100 + pc_checkskill(dstsd, MG_SRECOVERY) * 10 + pc_skillheal2_bonus(dstsd, skill_id)) / 100;
		}
		if (tsc != nullptr && !tsc->empty())
		{
			uint8 penalty = 0;

			if (tsc->getSCE(SC_WATER_INSIGNIA) && tsc->getSCE(SC_WATER_INSIGNIA)->val1 == 2)
			{
				hp += hp / 10;
				sp += sp / 10;
			}
			if (tsc->getSCE(SC_CRITICALWOUND))
				penalty += tsc->getSCE(SC_CRITICALWOUND)->val2;
			if (tsc->getSCE(SC_DEATHHURT) && tsc->getSCE(SC_DEATHHURT)->val3 == 1)
				penalty += 20;
			if (tsc->getSCE(SC_NORECOVER_STATE))
				penalty = 100;
			if (penalty > 0)
			{
				hp -= hp * penalty / 100;
				sp -= sp * penalty / 100;
			}
		}
		if (hp > 0)
			clif_skill_nodamage(nullptr, *bl, AL_HEAL, hp);
		if (sp > 0)
			clif_skill_nodamage(nullptr, *bl, MG_SRECOVERY, sp);
		status_heal(bl, hp, sp, 0);
	}
	break;
// Full Chemical Protection
case CR_FULLPROTECTION:
{
	uint32 equip[] = {EQP_WEAPON, EQP_SHIELD, EQP_ARMOR, EQP_HEAD_TOP};
	int32 i_eqp, s = 0, skilltime = skill_get_time(skill_id, skill_lv);

	for (i_eqp = 0; i_eqp < 4; i_eqp++)
	{
		if (bl->type != BL_PC || (dstsd && pc_checkequip(dstsd, equip[i_eqp]) < 0))
			continue;
		sc_start(src, bl, (sc_type)(SC_CP_WEAPON + i_eqp), 100, skill_lv, skilltime);
		s++;
	}
	if (sd && !s)
	{
		clif_skill_fail(*sd, skill_id);
		map_freeblock_unlock(); // Don't consume item requirements
		return 0;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
}
break;

case RG_CLEANER: // AppleGirl
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

#ifndef RENEWAL
case CG_LONGINGFREEDOM:
{
	if (tsc && !tsce && (tsce = tsc->getSCE(SC_DANCING)) && tsce->val4 && (tsce->val1 & 0xFFFF) != CG_MOONLIT) // Can't use Longing for Freedom while under Moonlight Petals. [Skotlex]
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv,
							sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
}
break;
#endif

case CG_TAROTCARD:
{
	int32 card = -1;
	if (tsc && tsc->getSCE(SC_TAROTCARD))
	{
		// Target currently has the SUN tarot card effect and is immune to any other effect
		map_freeblock_unlock();
		return 0;
	}
	if (rnd() % 100 > skill_lv * 8 ||
#ifndef RENEWAL
		(tsc && tsc->getSCE(SC_BASILICA)) ||
#endif
		(dstmd && ((dstmd->guardian_data && dstmd->mob_id == MOBID_EMPERIUM) || status_get_class_(bl) == CLASS_BATTLEFIELD)))
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);

		map_freeblock_unlock();
		return 0;
	}
	status_zap(src, 0, skill_get_sp(skill_id, skill_lv));	   // consume sp only if succeeded [Inkfish]
	card = skill_tarotcard(src, bl, skill_id, skill_lv, tick); // actual effect is executed here
	clif_specialeffect((card == 6) ? src : bl, EF_TAROTCARD1 + card - 1, AREA);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
}
break;

case SL_ALCHEMIST:
case SL_ASSASIN:
case SL_BARDDANCER:
case SL_BLACKSMITH:
case SL_CRUSADER:
case SL_HUNTER:
case SL_KNIGHT:
case SL_MONK:
case SL_PRIEST:
case SL_ROGUE:
case SL_SAGE:
case SL_SOULLINKER:
case SL_STAR:
case SL_SUPERNOVICE:
case SL_WIZARD:
case SL_HIGH:
	if (sc_start2(src, bl, type, 100, skill_lv, skill_id, skill_get_time(skill_id, skill_lv)))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);

		// 1% chance to erase death count on successful cast
		if (skill_id == SL_SUPERNOVICE && dstsd && dstsd->die_counter && rnd_chance(1, 100))
		{
			pc_setparam(dstsd, SP_PCDIECOUNTER, 0);
			clif_specialeffect(bl, EF_ANGEL2, AREA);
			status_calc_pc(dstsd, SCO_NONE);
		}

		sc_start(src, src, SC_SMA, 100, skill_lv, skill_get_time(SL_SMA, skill_lv));
	}
	else
	{
		if (sd)
		{
			clif_skill_fail(*sd, skill_id);
		}
	}
	break;
case SP_SOULGOLEM:
case SP_SOULSHADOW:
case SP_SOULFALCON:
case SP_SOULFAIRY:
	if (sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	else
	{
		if (sd)
		{
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
		}
	}
	break;

case SP_SOULREVOLVE:
	if (!(tsc && (tsc->getSCE(SC_SPIRIT) || tsc->getSCE(SC_SOULGOLEM) || tsc->getSCE(SC_SOULSHADOW) || tsc->getSCE(SC_SOULFALCON) || tsc->getSCE(SC_SOULFAIRY))))
	{
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
		break;
	}
	status_heal(bl, 0, 50 * skill_lv, 2);
	status_change_end(bl, SC_SPIRIT);
	status_change_end(bl, SC_SOULGOLEM);
	status_change_end(bl, SC_SOULSHADOW);
	status_change_end(bl, SC_SOULFALCON);
	status_change_end(bl, SC_SOULFAIRY);
	break;

case SL_SWOO:
	if (tsce)
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		status_change_start(src, src, SC_STUN, 10000, skill_lv, 0, 0, 0, 10000, SCSTART_NORATEDEF);
		status_change_end(bl, SC_SWOO);
		break;
	}
	[[fallthrough]];
case SL_SKA: // [marquis007]
case SL_SKE:
	if (sd && !battle_config.allow_es_magic_pc && bl->type != BL_MOB)
	{
		clif_skill_fail(*sd, skill_id);
		status_change_start(src, src, SC_STUN, 10000, skill_lv, 0, 0, 0, 500, SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
		break;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	if (skill_id == SL_SKE)
		sc_start(src, src, SC_SMA, 100, skill_lv, skill_get_time(SL_SMA, skill_lv));
	break;

case AG_ENERGY_CONVERSION:
	if (status_get_sp(src) == status_get_max_sp(src))
	{
		if (sd != nullptr)
		{
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
		}
		break;
	}

	// Apply the SP gain to the caster
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_heal(bl, 0, (skill_lv * (skill_lv + 1) / 2) * 40, 1);
	break;

case HN_OVERCOMING_CRISIS:
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_percent_heal(bl, 100, 0);
	break;

// New guild skills [Celest]
case GD_BATTLEORDER:
case GD_REGENERATION:
case GD_RESTORE:
case GD_EMERGENCY_MOVE:
	if (flag & 1)
	{
		if (status_get_guild_id(src) == status_get_guild_id(bl))
		{
			if (skill_id == GD_RESTORE)
				clif_skill_nodamage(src, *bl, AL_HEAL, status_percent_heal(bl, 90, 90));
			else
				sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		}
	}
	else if (status_get_guild_id(src))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, src,
							  skill_get_splash(skill_id, skill_lv), BL_PC,
							  src, skill_id, skill_lv, tick, flag | BCT_GUILD | 1,
							  skill_castend_nodamage_id);
		if (sd)
#ifdef RENEWAL
			skill_blockpc_start(*sd, skill_id, skill_get_cooldown(skill_id, skill_lv));
#else
			guild_block_skill(sd, skill_get_time2(skill_id, skill_lv));
#endif
	}
	break;
case GD_EMERGENCYCALL:
case GD_ITEMEMERGENCYCALL:
{
	int8 dx[9] = {-1, 1, 0, 0, -1, 1, -1, 1, 0};
	int8 dy[9] = {0, 0, 1, -1, 1, -1, -1, 1, 0};
	uint8 j = 0, calls = 0, called = 0;
	// i don't know if it actually summons in a circle, but oh well. ;P
	auto g = sd ? sd->guild : guild_search(status_get_guild_id(src));
	if (!g)
		break;

	if (skill_id == GD_ITEMEMERGENCYCALL)
		switch (skill_lv)
		{
		case 1:
			calls = 7;
			break;
		case 2:
			calls = 12;
			break;
		case 3:
			calls = 20;
			break;
		default:
			calls = 0;
			break;
		}

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	for (i = 0; i < g->guild.max_member && (!calls || (calls && called < calls)); i++, j++)
	{
		if (j > 8)
			j = 0;
		if ((dstsd = g->guild.member[i].sd) != nullptr && sd != dstsd && !dstsd->state.autotrade && !pc_isdead(dstsd))
		{
			if (dstsd->status.disable_call)
				continue;
			if (map_getmapflag(dstsd->m, MF_NOWARP) && !map_flag_gvg2(dstsd->m))
				continue;
			if (!pc_job_can_entermap((enum e_job)dstsd->status.class_, src->m, pc_get_group_level(dstsd)))
				continue;
			if (map_getcell(src->m, src->x + dx[j], src->y + dy[j], CELL_CHKNOREACH))
				dx[j] = dy[j] = 0;
			if (!pc_setpos(dstsd, map_id2index(src->m), src->x + dx[j], src->y + dy[j], CLR_RESPAWN))
				called++;
		}
	}
	if (sd)
#ifdef RENEWAL
		skill_blockpc_start(*sd, skill_id, skill_get_cooldown(skill_id, skill_lv));
#else
		guild_block_skill(sd, skill_get_time2(skill_id, skill_lv));
#endif
}
break;
case GD_CHARGESHOUT_FLAG:
	if (sd && sd->guild && sd->state.gmaster_flag == 1)
	{
		mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, sd->guild->guild.name, MOBID_GUILD_SKILL_FLAG, nullptr, SZ_SMALL, AI_GUILD);

		if (md)
		{
			sd->guild->chargeshout_flag_id = md->id;
			md->master_id = src->id;

			if (md->deletetimer != INVALID_TIMER)
				delete_timer(md->deletetimer, mob_timer_delete);
			md->deletetimer = add_timer(gettick() + skill_get_time(GD_CHARGESHOUT_FLAG, skill_lv), mob_timer_delete, md->id, 0);
			mob_spawn(md);
		}
	}
	break;
case GD_CHARGESHOUT_BEATING:
	if (sd && sd->guild && map_blid_exists(sd->guild->chargeshout_flag_id))
	{
		block_list *mob_bl = map_id2bl(sd->guild->chargeshout_flag_id);

		if (pc_setpos(sd, map_id2index(mob_bl->m), mob_bl->x, mob_bl->y, CLR_RESPAWN) != SETPOS_OK)
			clif_skill_fail(*sd, skill_id);
		else
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
	break;

case SG_FEEL:
	// AuronX reported you CAN memorize the same map as all three. [Skotlex]
	if (sd)
	{
		if (!sd->feel_map[skill_lv - 1].index)
			clif_feel_req(sd->fd, sd, skill_lv);
		else
			clif_feel_info(sd, skill_lv - 1, 1);
	}
	break;

case SG_HATE:
	if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		if (!pc_set_hate_mob(sd, skill_lv - 1, bl))
			clif_skill_fail(*sd, skill_id);
	}
	break;

case SJ_DOCUMENT:
	if (sd)
	{
		switch (skill_lv)
		{
		case 1:
			pc_resetfeel(sd);
			break;
		case 2:
			pc_resethate(sd);
			break;
		case 3:
			pc_resetfeel(sd);
			pc_resethate(sd);
			break;
		}
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case GS_GLITTERING:
	if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		if (rnd() % 100 < (20 + 10 * skill_lv))
			pc_addspiritball(sd, skill_get_time(skill_id, skill_lv), 10);
		else if (sd->spiritball > 0 && !pc_checkskill(sd, RL_RICHS_COIN))
			pc_delspiritball(sd, 1, 0);
	}
	break;

case GS_CRACKER:
	/* per official standards, this skill works on players and mobs. */
	if (sd && (dstsd || dstmd))
	{
		i = 65 - 5 * distance_bl(src, bl); // Base rate
		if (i < 30)
			i = 30;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		sc_start(src, bl, SC_STUN, i, skill_lv, skill_get_time2(skill_id, skill_lv));
	}
	break;

case AM_CALLHOMUN: //[orn]
	if (sd && !hom_call(sd))
		clif_skill_fail(*sd, skill_id);
#ifdef RENEWAL
	else if (sd && hom_is_active(sd->hd))
		skill_area_temp[0] = 1; // Already passed pre-cast checks
#endif
	break;

case AM_REST:
	if (sd)
	{
		if (hom_vaporize(sd, HOM_ST_REST))
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		else
			clif_skill_fail(*sd, skill_id);
	}
	break;

case HAMI_CASTLE: //[orn]
	if (src != bl && rnd_chance(20 * skill_lv, 100))
	{
		// Get one of the monsters targeting the player and set the homunculus as its new target
		if (block_list *tbl = battle_gettargeted(bl); tbl != nullptr && tbl->type == BL_MOB)
		{
			if (unit_data *ud = unit_bl2ud(tbl); ud != nullptr)
				unit_changetarget_sub(*ud, *src);
		}

		int16 x = src->x, y = src->y;
		// Move homunculus
		if (unit_movepos(src, bl->x, bl->y, 0, false))
		{
			clif_blown(src);
			// Move player
			if (unit_movepos(bl, x, y, 0, false))
				clif_blown(bl);
			// Show the animation on the homunculus only
			clif_skill_nodamage(src, *src, skill_id, skill_lv);
		}
	}
	else if (hd != nullptr && hd->master != nullptr)
		clif_skill_fail(*hd->master, skill_id);
	else if (sd != nullptr)
		clif_skill_fail(*sd, skill_id);
	break;
case HVAN_CHAOTIC:
{
	// Chance per skill level
	static const std::array<uint8, 5> chance_homunculus = {
		20,
		50,
		25,
		50,
		34};
	static const std::array<uint8, 5> chance_master = {
		static_cast<uint8>(chance_homunculus[0] + 30),
		static_cast<uint8>(chance_homunculus[1] + 10),
		static_cast<uint8>(chance_homunculus[2] + 50),
		static_cast<uint8>(chance_homunculus[3] + 4),
		static_cast<uint8>(chance_homunculus[4] + 33)};

	uint8 chance = rnd_value(1, 100);

	// Homunculus
	if (chance <= chance_homunculus[skill_lv - 1])
		bl = src;
	// Master
	else if (chance <= chance_master[skill_lv - 1])
		bl = battle_get_master(src);
	// Enemy (A random enemy targeting the master)
	else
		bl = battle_gettargeted(battle_get_master(src));

	// If there's no enemy the chance reverts to the homunculus
	if (bl == nullptr)
		bl = src;

	int32 heal = skill_calc_heal(src, bl, skill_id, rnd_value<uint16>(1, skill_lv), true);

	// Official servers send the Heal skill packet with the healed amount, and then the skill packet with 1 as healed amount
	clif_skill_nodamage(src, *bl, AL_HEAL, heal);
	clif_skill_nodamage(src, *bl, skill_id, 1);
	status_heal(bl, heal, 0, 0);
}
break;
case HVAN_EXPLOSION:
	if (hd != nullptr)
	{
		clif_skill_nodamage(src, *src, skill_id, skill_lv, 1);
		map_foreachinshootrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY, skill_castend_damage_id);

		hd->homunculus.intimacy = hom_intimacy_grade2intimacy(HOMGRADE_HATE_WITH_PASSION);
		clif_send_homdata(*hd, SP_INTIMATE);

		// There's a delay between the explosion and the homunculus death
		skill_addtimerskill(src, tick + skill_get_time(skill_id, skill_lv), src->id, 0, 0, skill_id, skill_lv, 0, flag);
	}
	break;
// Homun single-target support skills [orn]
case HLIF_CHANGE:
#ifndef RENEWAL
	status_percent_heal(bl, 100, 100);
	[[fallthrough]];
#endif
case HAMI_BLOODLUST:
case HFLI_FLEET:
case HFLI_SPEED:
case MH_ANGRIFFS_MODUS:
case MH_GOLDENE_FERSE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case NPC_DRAGONFEAR:
	if (flag & 1)
	{
		const enum sc_type sc[] = {SC_STUN, SC_SILENCE, SC_CONFUSION, SC_BLEEDING};
		int32 j;
		j = i = rnd() % ARRAYLENGTH(sc);
		while (!sc_start2(src, bl, sc[i], 100, skill_lv, src->id, skill_get_time2(skill_id, i + 1)))
		{
			i++;
			if (i == ARRAYLENGTH(sc))
				i = 0;
			if (i == j)
				break;
		}
		break;
	}
	[[fallthrough]];
case NPC_WIDEBLEEDING:
case NPC_WIDEBLEEDING2:
case NPC_WIDECONFUSE:
case NPC_WIDECONFUSE2:
case NPC_WIDECURSE:
case NPC_WIDECURSE2:
case NPC_WIDEFREEZE:
case NPC_WIDEFREEZE2:
case NPC_WIDESLEEP:
case NPC_WIDESLEEP2:
case NPC_WIDESILENCE:
case NPC_WIDESILENCE2:
case NPC_WIDESTONE:
case NPC_WIDESTONE2:
case NPC_WIDESTUN:
case NPC_WIDESTUN2:
case NPC_SLOWCAST:
case NPC_WIDEHELLDIGNITY:
case NPC_WIDEHEALTHFEAR:
case NPC_WIDEBODYBURNNING:
case NPC_WIDEFROSTMISTY:
case NPC_WIDECOLD:
case NPC_WIDE_DEEP_SLEEP:
case NPC_WIDESIREN:
case NPC_WIDEWEB:
case NPC_DEADLYCURSE2:
	if (flag & 1)
	{
		switch (type)
		{
		case SC_BURNING:
			sc_start4(src, bl, type, 100, skill_lv, 1000, src->id, 0, skill_get_time2(skill_id, skill_lv));
			break;
		case SC_STONEWAIT:
			sc_start2(src, bl, type, 100, skill_lv, src->id, skill_get_time2(skill_id, skill_lv), skill_get_time(skill_id, skill_lv));
			break;
		default:
			sc_start2(src, bl, type, 100, skill_lv, src->id, skill_get_time2(skill_id, skill_lv));
		}
	}
	else
	{
		skill_area_temp[2] = 0; // For SD_PREAMBLE
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, bl,
							  skill_get_splash(skill_id, skill_lv), BL_CHAR,
							  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_PREAMBLE | 1,
							  skill_castend_nodamage_id);
	}
	break;
case NPC_WIDESOULDRAIN:
	if (flag & 1)
		status_percent_damage(src, bl, 0, ((skill_lv - 1) % 5 + 1) * 20, false);
	else
	{
		skill_area_temp[2] = 0; // For SD_PREAMBLE
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, bl,
							  skill_get_splash(skill_id, skill_lv), BL_CHAR,
							  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_PREAMBLE | 1,
							  skill_castend_nodamage_id);
	}
	break;
case NPC_FIRESTORM:
{
	int32 sflag = flag;

	if (skill_lv > 1)
		sflag |= 4;
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_foreachinshootrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), splash_target(src), src,
							skill_id, skill_lv, tick, sflag | BCT_ENEMY | SD_ANIMATION | 1, skill_castend_damage_id);
}
break;
case ALL_PARTYFLEE:
	if (sd && !(flag & 1))
	{
		if (!sd->status.party_id)
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
	else
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
case NPC_TALK:
case ALL_WEWISH:
case ALL_CATCRY:
case ALL_DREAM_SUMMERNIGHT:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case ALL_BUYING_STORE:
	if (sd)
	{ // players only, skill allows 5 buying slots
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, buyingstore_setup(sd, MAX_BUYINGSTORE_SLOTS) == 0);
	}
	break;
case RK_ENCHANTBLADE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start2(src, bl, type, 100, skill_lv, ((100 + 20 * skill_lv) * status_get_lv(src)) / 100 + sstatus->int_, skill_get_time(skill_id, skill_lv)));
	break;
case RK_DRAGONHOWLING:
	if (flag & 1)
		sc_start(src, bl, type, 50 + 6 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	else
	{
		skill_area_temp[2] = 0;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, src,
							  skill_get_splash(skill_id, skill_lv), BL_CHAR,
							  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_PREAMBLE | 1,
							  skill_castend_nodamage_id);
	}
	break;

case LG_EARTHDRIVE:
{
	int32 dummy = 1;

	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	i = skill_get_splash(skill_id, skill_lv);
	map_foreachinallarea(skill_cell_overlap, src->m, src->x - i, src->y - i, src->x + i, src->y + i, BL_SKILL, LG_EARTHDRIVE, &dummy, src);
	map_foreachinrange(skill_area_sub, bl, i, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
}
break;
case RK_LUXANIMA:
	status_change_clear_buffs(bl, SCCB_LUXANIMA); // For bonus_script
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
case RK_GIANTGROWTH:
case RK_STONEHARDSKIN:
case RK_VITALITYACTIVATION:
case RK_ABUNDANCE:
case RK_CRUSHSTRIKE:
case RK_REFRESH:
case RK_MILLENNIUMSHIELD:
	if (sd)
	{
		uint8 rune_level = 1; // RK_GIANTGROWTH

		if (skill_id == RK_VITALITYACTIVATION)
			rune_level = 2;
		else if (skill_id == RK_STONEHARDSKIN)
			rune_level = 4;
		else if (skill_id == RK_ABUNDANCE)
			rune_level = 6;
		else if (skill_id == RK_CRUSHSTRIKE)
			rune_level = 7;
		else if (skill_id == RK_REFRESH)
			rune_level = 8;
		else if (skill_id == RK_MILLENNIUMSHIELD)
			rune_level = 9;
		if (pc_checkskill(sd, RK_RUNEMASTERY) >= rune_level)
		{
			if (sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)))
				clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			else if (skill_id == RK_STONEHARDSKIN)
				clif_skill_fail(*sd, skill_id, USESKILL_FAIL_HP_INSUFFICIENT);
		}
		else
			clif_skill_fail(*sd, skill_id);
	}
	break;

case NPC_MILLENNIUMSHIELD:
	if (sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)))
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case RK_FIGHTINGSPIRIT:
{
	uint8 runemastery_skill_lv = (sd ? pc_checkskill(sd, RK_RUNEMASTERY) : skill_get_max(RK_RUNEMASTERY));

	// val1: ATKBonus: ? // !TODO: Confirm new ATK formula
	// val2: ASPD boost: [RK_RUNEMASTERYlevel * 4 / 10] * 10 ==> RK_RUNEMASTERYlevel * 4
	sc_start2(src, bl, type, 100, 70 + 7 * runemastery_skill_lv, 4 * runemastery_skill_lv, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;
}

case GC_ROLLINGCUTTER:
{
	int16 count = 1;
	skill_area_temp[2] = 0;
	map_foreachinrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_PREAMBLE | SD_SPLASH | 1, skill_castend_damage_id);
	if (tsc && tsc->getSCE(SC_ROLLINGCUTTER))
	{ // Every time the skill is casted the status change is reseted adding a counter.
		count += (int16)tsc->getSCE(SC_ROLLINGCUTTER)->val1;
		if (count > 10)
			count = 10; // Max coounter
		status_change_end(bl, SC_ROLLINGCUTTER);
	}
	sc_start(src, bl, SC_ROLLINGCUTTER, 100, count, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
}
break;

case GC_WEAPONBLOCKING:
	if (tsc && tsc->getSCE(SC_WEAPONBLOCKING))
		status_change_end(bl, SC_WEAPONBLOCKING);
	else
		sc_start(src, bl, SC_WEAPONBLOCKING, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case GC_CREATENEWPOISON:
	if (sd)
	{
		clif_skill_produce_mix_list(*sd, skill_id, 25);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case GC_POISONINGWEAPON:
	if (sd)
	{
		clif_poison_list(*sd, skill_lv);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case GC_ANTIDOTE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (tsc)
	{
		status_change_end(bl, SC_PARALYSE);
		status_change_end(bl, SC_PYREXIA);
		status_change_end(bl, SC_DEATHHURT);
		status_change_end(bl, SC_LEECHESEND);
		status_change_end(bl, SC_VENOMBLEED);
		status_change_end(bl, SC_MAGICMUSHROOM);
		status_change_end(bl, SC_TOXIN);
		status_change_end(bl, SC_OBLIVIONCURSE);
	}
	break;

case GC_PHANTOMMENACE:
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_foreachinrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR,
					   src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	break;

case GC_HALLUCINATIONWALK:
{
	int32 heal = status_get_max_hp(bl) / 10;
	if (status_get_hp(bl) < heal)
	{ // if you haven't enough HP skill fails.
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL_HP_INSUFFICIENT);
		break;
	}
	if (!status_charge(bl, heal, 0))
	{
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL_HP_INSUFFICIENT);
		break;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
}
break;

case AB_ANCILLA:
	if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		skill_produce_mix(sd, skill_id, ITEMID_ANCILLA, 0, 0, 0, 1, -1);
	}
	break;

case AB_CLEMENTIA:
case AB_CANTO:
{
	int32 bless_lv = ((sd) ? pc_checkskill(sd, AL_BLESSING) : skill_get_max(AL_BLESSING)) + (((sd) ? sd->status.job_level : 50) / 10);
	int32 agi_lv = ((sd) ? pc_checkskill(sd, AL_INCAGI) : skill_get_max(AL_INCAGI)) + (((sd) ? sd->status.job_level : 50) / 10);
	if (sd == nullptr || sd->status.party_id == 0 || flag & 1)
		clif_skill_nodamage(bl, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, (skill_id == AB_CLEMENTIA) ? bless_lv : (skill_id == AB_CANTO) ? agi_lv
																																					 : skill_lv,
																  skill_get_time(skill_id, skill_lv)));
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
}
break;

case AB_PRAEFATIO:
case AB_RENOVATIO:
	if (!sd || sd->status.party_id == 0 || flag & 1)
	{
		if (skill_id == AB_PRAEFATIO)
			clif_skill_nodamage(bl, *bl, skill_id, skill_lv, sc_start4(src, bl, type, 100, skill_lv, 0, 0, (sd && sd->status.party_id ? party_foreachsamemap(party_sub_count, sd, 0) : 1), skill_get_time(skill_id, skill_lv)));
		else
			clif_skill_nodamage(bl, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	break;

case AB_CHEAL:
	if (!sd || sd->status.party_id == 0 || flag & 1)
	{
		if (sd && tstatus && !battle_check_undead(tstatus->race, tstatus->def_ele) && !tsc->getSCE(SC_BERSERK))
		{
			int32 partycount = (sd->status.party_id ? party_foreachsamemap(party_sub_count, sd, 0) : 0);

			i = skill_calc_heal(src, bl, AL_HEAL, pc_checkskill(sd, AL_HEAL), true);

			if (partycount > 1)
				i += (i / 100) * (partycount * 10) / 4;
			if ((dstsd && pc_ismadogear(dstsd)) || status_isimmune(bl))
				i = 0; // Should heal by 0 or won't do anything?? in iRO it breaks the healing to members.. [malufett]

			clif_skill_nodamage(src, *bl, skill_id, i);
			if (tsc && tsc->getSCE(SC_AKAITSUKI) && i)
				i = ~i + 1;
			status_heal(bl, i, 0, 0);
		}
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	break;

case NPC_CHEAL:
	if (flag & 1)
	{
		if (tstatus && !battle_check_undead(tstatus->race, tstatus->def_ele) && !tsc->getSCE(SC_BERSERK))
		{
			i = skill_calc_heal(src, bl, AL_HEAL, 10, true);
			if (status_isimmune(bl))
				i = 0;
			clif_skill_nodamage(src, *bl, skill_id, i);
			if (tsc && tsc->getSCE(SC_AKAITSUKI) && i)
				i = ~i + 1;
			status_heal(bl, i, 0, 0);
		}
	}
	else
	{
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_MOB,
							  src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case AB_ORATIO:
	if (flag & 1)
		sc_start(src, bl, type, 40 + 5 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	else
	{
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR,
							  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case AB_LAUDAAGNUS:
	if (flag & 1 || !sd || !sd->status.party_id)
	{
		if (tsc && (tsc->getSCE(SC_FREEZE) || tsc->getSCE(SC_STONE) || tsc->getSCE(SC_BLIND) ||
					tsc->getSCE(SC_BURNING) || tsc->getSCE(SC_FREEZING) || tsc->getSCE(SC_CRYSTALIZE)))
		{
			// Success Chance: (60 + 10 * Skill Level) %
			if (rnd() % 100 > 60 + 10 * skill_lv)
				break;
			status_change_end(bl, SC_FREEZE);
			status_change_end(bl, SC_STONE);
			status_change_end(bl, SC_BLIND);
			status_change_end(bl, SC_BURNING);
			status_change_end(bl, SC_FREEZING);
			status_change_end(bl, SC_CRYSTALIZE);
		}
		else // Success rate only applies to the curing effect and not stat bonus. Bonus status only applies to non infected targets
			clif_skill_nodamage(bl, *bl, skill_id, skill_lv,
								sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv),
							 src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	break;

case AB_LAUDARAMUS:
	if (flag & 1 || !sd || !sd->status.party_id)
	{
		if (tsc && (tsc->getSCE(SC_SLEEP) || tsc->getSCE(SC_STUN) || tsc->getSCE(SC_MANDRAGORA) || tsc->getSCE(SC_SILENCE) || tsc->getSCE(SC_DEEPSLEEP)))
		{
			// Success Chance: (60 + 10 * Skill Level) %
			if (rnd() % 100 > 60 + 10 * skill_lv)
				break;
			status_change_end(bl, SC_SLEEP);
			status_change_end(bl, SC_STUN);
			status_change_end(bl, SC_MANDRAGORA);
			status_change_end(bl, SC_SILENCE);
			status_change_end(bl, SC_DEEPSLEEP);
		}
		else // Success rate only applies to the curing effect and not stat bonus. Bonus status only applies to non infected targets
			clif_skill_nodamage(bl, *bl, skill_id, skill_lv,
								sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv),
							 src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	break;

case AB_CLEARANCE:
	if (flag & 1 || (i = skill_get_splash(skill_id, skill_lv)) < 1)
	{ // As of the behavior in official server Clearance is just a super version of Dispell skill. [Jobbie]

		if (bl->type != BL_MOB && battle_check_target(src, bl, BCT_PARTY) <= 0) // Only affect mob or party.
			break;

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);

		if (rnd() % 100 >= 60 + 8 * skill_lv)
		{
			if (sd)
				clif_skill_fail(*sd, skill_id);
			break;
		}

		if (status_isimmune(bl))
			break;

		// Remove bonus_script by Clearance
		if (dstsd)
			pc_bonus_script_clear(dstsd, BSF_REM_ON_CLEARANCE);

		if (tsc == nullptr || tsc->empty())
			break;

		// Statuses change that can't be removed by Cleareance
		for (const auto &it : status_db)
		{
			sc_type status = static_cast<sc_type>(it.first);

			if (!tsc->getSCE(status))
				continue;

			if (it.second->flag[SCF_NOCLEARANCE])
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
			if (i == SC_BERSERK || i == SC_SATURDAYNIGHTFEVER)
				tsc->getSCE(status)->val2 = 0; // Mark a dispelled berserk to avoid setting hp to 100 by setting hp penalty to 0.
			status_change_end(bl, status);
		}
		break;
	}

	map_foreachinallrange(skill_area_sub, bl, i, BL_CHAR, src, skill_id, skill_lv, tick, flag | 1, skill_castend_damage_id);
	break;

case AB_SILENTIUM:
	// Should the level of Lex Divina be equivalent to the level of Silentium or should the highest level learned be used? [LimitLine]
	map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR,
						  src, PR_LEXDIVINA, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case WL_STASIS:
	if (flag & 1)
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	else
	{
		struct map_data *mapdata = map_getmapdata(src->m);

		map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, (mapdata_flag_vs(mapdata) ? BCT_ALL : BCT_ENEMY | BCT_SELF) | flag | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case NPC_DANCINGBLADE:
	skill_addtimerskill(src, tick + status_get_amotion(src), bl->id, 0, 0, NPC_DANCINGBLADE_ATK, skill_lv, 0, 0);
	break;

case WL_CHAINLIGHTNING:
	skill_addtimerskill(src, tick + status_get_amotion(src), bl->id, 0, 0, WL_CHAINLIGHTNING_ATK, skill_lv, 0, 0);
	break;

case WL_WHITEIMPRISON:
	if ((src == bl || battle_check_target(src, bl, BCT_ENEMY) > 0) && status_get_class_(bl) != CLASS_BOSS && !status_isimmune(bl)) // Should not work with Bosses.
	{
		int32 rate = (sd ? sd->status.job_level : 50) / 4;

		if (src == bl)
			rate = 100; // Success Chance: On self, 100%
		else if (bl->type == BL_PC)
			rate += 20 + 10 * skill_lv; // On Players, (20 + 10 * Skill Level) %
		else
			rate += 40 + 10 * skill_lv; // On Monsters, (40 + 10 * Skill Level) %

		if (sd)
			skill_blockpc_start(*sd, skill_id, 4000);

		if (!(tsc && tsc->getSCE(type)))
		{
			i = sc_start2(src, bl, type, rate, skill_lv, src->id, (src == bl) ? 5000 : (bl->type == BL_PC) ? skill_get_time(skill_id, skill_lv)
																										   : skill_get_time2(skill_id, skill_lv));
			clif_skill_nodamage(src, *bl, skill_id, skill_lv, i);
			if (sd && !i)
				clif_skill_fail(*sd, skill_id);
		}
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL_TOTARGET);
	break;

case NPC_JACKFROST:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	break;

case WL_SIENNAEXECRATE:
	if (status_isimmune(bl) || !tsc)
		break;

	if (flag & 1)
	{
		if (bl->id == skill_area_temp[1])
			break; // Already work on this target

		status_change_start(src, bl, type, 10000, skill_lv, src->id, 0, 0, skill_get_time2(skill_id, skill_lv), SCSTART_NOTICKDEF, skill_get_time(skill_id, skill_lv));
	}
	else
	{
		int32 rate = 45 + 5 * skill_lv + (sd ? sd->status.job_level : 50) / 4;
		// IroWiki says Rate should be reduced by target stats, but currently unknown
		if (rnd() % 100 < rate)
		{ // Success on First Target
			if (status_change_start(src, bl, type, 10000, skill_lv, src->id, 0, 0, skill_get_time2(skill_id, skill_lv), SCSTART_NOTICKDEF, skill_get_time(skill_id, skill_lv)))
			{
				clif_skill_nodamage(src, *bl, skill_id, skill_lv);
				skill_area_temp[1] = bl->id;
				map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
			}
			// Doesn't send failure packet if it fails on defense.
		}
		else if (sd) // Failure on Rate
			clif_skill_fail(*sd, skill_id);
	}
	break;

case WL_SUMMONFB:
case WL_SUMMONBL:
case WL_SUMMONWB:
case WL_SUMMONSTONE:
{
	if (sc == nullptr)
		break;

	e_wl_spheres element;

	switch (skill_id)
	{ // Set val2. The SC element for this ball
	case WL_SUMMONFB:
		element = WLS_FIRE;
		break;
	case WL_SUMMONBL:
		element = WLS_WIND;
		break;
	case WL_SUMMONWB:
		element = WLS_WATER;
		break;
	case WL_SUMMONSTONE:
		element = WLS_STONE;
		break;
	}

	if (skill_lv == 1)
	{
		sc_type sphere = SC_NONE;

		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++)
		{
			if (sc->getSCE(i) == nullptr)
			{
				sphere = static_cast<sc_type>(i); // Take the free SC
				break;
			}
		}

		if (sphere == SC_NONE)
		{
			if (sd) // No free slots to put SC
				clif_skill_fail(*sd, skill_id, USESKILL_FAIL_SUMMON);
			break;
		}

		sc_start2(src, src, sphere, 100, element, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else
	{
		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++)
		{
			status_change_end(src, static_cast<sc_type>(i)); // Removes previous type
			sc_start2(src, src, static_cast<sc_type>(i), 100, element, skill_lv, skill_get_time(skill_id, skill_lv));
		}
	}

	clif_skill_nodamage(src, *bl, skill_id, 0, false);
}
break;

case WL_READING_SB_READING:
	if (sd)
	{
		if (pc_checkskill(sd, WL_READING_SB) == 0 || skill_lv < 1 || skill_lv > 10)
		{
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL_SPELLBOOK_READING);
			break;
		}

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		skill_spellbook(*sd, ITEMID_WL_MB_SG + skill_lv - 1);
	}
	break;

case RA_FEARBREEZE:
	clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case RA_WUGMASTERY:
	if (sd)
	{
		if (!pc_iswug(sd))
			pc_setoption(sd, sd->sc.option | OPTION_WUG);
		else
			pc_setoption(sd, sd->sc.option & ~OPTION_WUG);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case RA_WUGRIDER:
	if (sd)
	{
		if (!pc_isridingwug(sd) && pc_iswug(sd))
		{
			pc_setoption(sd, sd->sc.option & ~OPTION_WUG);
			pc_setoption(sd, sd->sc.option | OPTION_WUGRIDER);
		}
		else if (pc_isridingwug(sd))
		{
			pc_setoption(sd, sd->sc.option & ~OPTION_WUGRIDER);
			pc_setoption(sd, sd->sc.option | OPTION_WUG);
		}
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case RA_WUGDASH:
	if (tsce)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, status_change_end(bl, type));
		map_freeblock_unlock();
		return 0;
	}
	if (sd && pc_isridingwug(sd))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start4(src, bl, type, 100, skill_lv, unit_getdir(bl), 0, 0, 0));
		clif_walkok(*sd);
	}
	break;

case RA_SENSITIVEKEEN:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	map_foreachinrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY, skill_castend_damage_id);
	break;

case NC_F_SIDESLIDE:
case NC_B_SIDESLIDE:
{
	uint8 dir = (skill_id == NC_F_SIDESLIDE) ? (unit_getdir(src) + 4) % 8 : unit_getdir(src);
	skill_blown(src, bl, skill_get_blewcount(skill_id, skill_lv), dir, BLOWN_IGNORE_NO_KNOCKBACK);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
}
break;

case NC_SELFDESTRUCTION:
	if (sd)
	{
		if (pc_ismadogear(sd))
			pc_setmadogear(sd, false);
		skill_area_temp[1] = 0;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		status_set_sp(src, 0, 0);
		skill_clear_unitgroup(src);
	}
	break;

case NC_EMERGENCYCOOL:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd)
	{
		struct s_skill_condition req = skill_get_requirement(sd, skill_id, skill_lv);
		int16 limit[] = {-45, -75, -105};

		i = 0;
		for (const auto &reqItem : req.eqItem)
		{
			if (pc_search_inventory(sd, reqItem) != -1)
				break;
			i++;
		}
		pc_overheat(*sd, limit[min(i, 2)]);
	}
	break;

case NC_ANALYZE:
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	sc_start(src, bl, type, 30 + 12 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case NC_MAGNETICFIELD:
	if (flag & 1)
	{
		sc_start2(src, bl, SC_MAGNETICFIELD, 100, skill_lv, src->id, skill_get_time(skill_id, skill_lv));
	}
	else
	{
		if (map_flag_vs(src->m)) // Doesn't affect the caster in non-PVP maps [exneval]
			sc_start2(src, bl, type, 100, skill_lv, src->id, skill_get_time(skill_id, skill_lv));
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), splash_target(src), src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_nodamage_id);
		clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	}
	break;

case NC_REPAIR:
	if (sd)
	{
		int32 heal, hp = 0;
		if (!dstsd || !pc_ismadogear(dstsd))
		{
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL_TOTARGET);
			break;
		}
		switch (skill_lv)
		{
		case 1:
			hp = 4;
			break;
		case 2:
			hp = 7;
			break;
		case 3:
			hp = 13;
			break;
		case 4:
			hp = 17;
			break;
		case 5:
		default:
			hp = 23;
			break;
		}
		heal = dstsd->status.max_hp * hp / 100;
		status_heal(bl, heal, 0, 2);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, heal != 0);
	}
	break;

case NC_DISJOINT:
{
	if (bl->type != BL_MOB)
		break;
	md = map_id2md(bl->id);
	if (md && md->mob_id >= MOBID_SILVERSNIPER && md->mob_id <= MOBID_MAGICDECOY_WIND)
		status_kill(bl);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
}
break;
case SC_AUTOSHADOWSPELL:
	if (sd)
	{
		if ((sd->reproduceskill_idx > 0 && sd->status.skill[sd->reproduceskill_idx].id) ||
			(sd->cloneskill_idx > 0 && sd->status.skill[sd->cloneskill_idx].id))
		{
			sc_start(src, src, SC_STOP, 100, skill_lv, INFINITE_TICK); // The skill_lv is stored in val1 used in skill_select_menu to determine the used skill lvl [Xazax]
			clif_autoshadowspell_list(*sd);
			clif_skill_nodamage(src, *bl, skill_id, 1);
		}
		else
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL_IMITATION_SKILL_NONE);
	}
	break;

case SC_SHADOWFORM:
	if (sd && dstsd && src != bl && !dstsd->shadowform_id)
	{
		if (clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start4(src, src, type, 100, skill_lv, bl->id, 4 + skill_lv, 0, skill_get_time(skill_id, skill_lv))))
			dstsd->shadowform_id = src->id;
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
	break;

case SC_BODYPAINT:
	if (flag & 1)
	{
		if (tsc && ((tsc->option & (OPTION_HIDE | OPTION_CLOAK)) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD)))
		{
			status_change_end(bl, SC_HIDING);
			status_change_end(bl, SC_CLOAKING);
			status_change_end(bl, SC_CLOAKINGEXCEED);
			status_change_end(bl, SC_CAMOUFLAGE);
			status_change_end(bl, SC_NEWMOON);
			if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
				status_change_end(bl, SC__SHADOWFORM);
		}
		// Attack Speed decrease and Blind happen to everyone around caster, not just hidden targets.
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		sc_start(src, bl, SC_BLIND, 53 + 2 * skill_lv, skill_lv, skill_get_time2(skill_id, skill_lv));
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, 0);
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR,
							  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
	break;

case SC_ENERVATION:
case SC_GROOMY:
case SC_LAZINESS:
case SC_UNLUCKY:
case SC_WEAKNESS:
	if (!(tsc && tsc->getSCE(type)))
	{
		int32 rate;

		if (status_get_class_(bl) == CLASS_BOSS)
			break;
		rate = status_get_lv(src) / 10 + rnd_value(sstatus->dex / 12, sstatus->dex / 4) + (sd ? sd->status.job_level : 50) + 10 * skill_lv - (status_get_lv(bl) / 10 + rnd_value(tstatus->agi / 6, tstatus->agi / 3) + tstatus->luk / 10 + (dstsd ? (dstsd->max_weight / 10 - dstsd->weight / 10) / 100 : 0));
		rate = cap_value(rate, skill_lv + sstatus->dex / 20, 100);
		clif_skill_nodamage(src, *bl, skill_id, 0, sc_start(src, bl, type, rate, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
	break;

case SC_IGNORANCE:
	if (!(tsc && tsc->getSCE(type)))
	{
		int32 rate;

		if (status_get_class_(bl) == CLASS_BOSS)
			break;
		rate = status_get_lv(src) / 10 + rnd_value(sstatus->dex / 12, sstatus->dex / 4) + (sd ? sd->status.job_level : 50) + 10 * skill_lv - (status_get_lv(bl) / 10 + rnd_value(tstatus->agi / 6, tstatus->agi / 3) + tstatus->luk / 10 + (dstsd ? (dstsd->max_weight / 10 - dstsd->weight / 10) / 100 : 0));
		rate = cap_value(rate, skill_lv + sstatus->dex / 20, 100);
		if (clif_skill_nodamage(src, *bl, skill_id, 0, sc_start(src, bl, type, rate, skill_lv, skill_get_time(skill_id, skill_lv))))
		{
			int32 sp = 100 * skill_lv;

			if (dstmd)
				sp = dstmd->level;
			if (!dstmd)
				status_zap(bl, 0, sp);

			status_heal(src, 0, sp / 2, 3);
		}
		else if (sd)
			clif_skill_fail(*sd, skill_id);
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
	break;

case LG_TRAMPLE:
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	if (rnd() % 100 < (25 + 25 * skill_lv))
		map_foreachinallrange(skill_destroy_trap, bl, skill_get_splash(skill_id, skill_lv), BL_SKILL, tick);
	status_change_end(bl, SC_SV_ROOTTWIST);
	break;

case LG_REFLECTDAMAGE:
	if (tsc && tsc->getSCE(type))
		status_change_end(bl, type);
	else
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case LG_PIETY:
	if (flag & 1)
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	else
	{
		skill_area_temp[2] = 0;
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_PC, src, skill_id, skill_lv, tick, flag | SD_PREAMBLE | BCT_PARTY | BCT_SELF | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case SR_CURSEDCIRCLE:
	if (flag & 1)
	{
		if (status_get_class_(bl) == CLASS_BOSS)
			break;
		if (sc_start2(src, bl, type, 100, skill_lv, src->id, skill_get_time(skill_id, skill_lv)))
		{
			if (bl->type == BL_MOB)
				mob_unlocktarget((TBL_MOB *)bl, gettick());
			clif_bladestop(*src, bl->id, true);
			map_freeblock_unlock();
			return 1;
		}
	}
	else
	{
		int32 count = 0;
		clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
		count = map_forcountinrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), (sd) ? sd->spiritball_old : 15, // Assume 15 spiritballs in non-charactors
									BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		if (sd)
			pc_delspiritball(sd, count, 0);
		clif_skill_nodamage(src, *src, skill_id, skill_lv,
							sc_start2(src, src, SC_CURSEDCIRCLE_ATKER, 100, skill_lv, count, skill_get_time(skill_id, skill_lv)));
	}
	break;

case NPC_SR_CURSEDCIRCLE:
	if (flag & 1)
	{
		if (status_get_class_(bl) == CLASS_BOSS)
			break;
		if (sc_start2(src, bl, type, 50, skill_lv, src->id, skill_get_time(skill_id, skill_lv)))
		{
			if (bl->type == BL_MOB)
				mob_unlocktarget((TBL_MOB *)bl, gettick());
			clif_bladestop(*src, bl->id, true);
			map_freeblock_unlock();
			return 1;
		}
	}
	else
	{
		int32 count = 0;
		clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
		count = map_forcountinrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), (sd) ? sd->spiritball_old : 15, // Assume 15 spiritballs in non-charactors
									BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		if (sd)
			pc_delspiritball(sd, count, 0);
		clif_skill_nodamage(src, *src, skill_id, skill_lv,
							sc_start2(src, src, SC_CURSEDCIRCLE_ATKER, 50, skill_lv, count, skill_get_time(skill_id, skill_lv)));
	}
	break;

case SR_RAISINGDRAGON:
	if (sd)
	{
		int16 max = 5 + skill_lv;
		sc_start(src, bl, SC_EXPLOSIONSPIRITS, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		for (i = 0; i < max; i++) // Don't call more than max available spheres.
			pc_addspiritball(sd, skill_get_time(skill_id, skill_lv), max);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	break;

case SR_ASSIMILATEPOWER:
	if (flag & 1)
	{
		i = 0;
		if (dstsd && (sd == dstsd || map_flag_vs(src->m)) && (dstsd->class_ & MAPID_BASEMASK) != MAPID_GUNSLINGER)
		{
			if (dstsd->spiritball > 0)
			{
				i = dstsd->spiritball;
				pc_delspiritball(dstsd, dstsd->spiritball, 0);
			}
			if (dstsd->spiritcharm_type != CHARM_TYPE_NONE && dstsd->spiritcharm > 0)
			{
				i += dstsd->spiritcharm;
				pc_delspiritcharm(dstsd, dstsd->spiritcharm, dstsd->spiritcharm_type);
			}
		}
		if (i)
			status_percent_heal(src, 0, i);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, i != 0);
	}
	else
	{
		clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), splash_target(src), src, skill_id, skill_lv, tick, flag | BCT_ENEMY | BCT_SELF | SD_SPLASH | 1, skill_castend_nodamage_id);
	}
	break;

case SR_POWERVELOCITY:
	if (!dstsd)
		break;
	if (sd && dstsd->spiritball <= 5)
	{
		for (i = 0; i <= 5; i++)
		{
			pc_addspiritball(dstsd, skill_get_time(MO_CALLSPIRITS, pc_checkskill(sd, MO_CALLSPIRITS)), i);
			pc_delspiritball(sd, sd->spiritball, 0);
		}
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case SR_GENTLETOUCH_CURE:
{
	uint32 heal;

	if (dstmd && (dstmd->mob_id == MOBID_EMPERIUM || status_get_class_(bl) == CLASS_BATTLEFIELD))
		heal = 0;
	else
	{
		heal = (120 * skill_lv) + (status_get_max_hp(bl) * skill_lv / 100);
		status_heal(bl, heal, 0, 0);
	}

	if (tsc != nullptr && !tsc->empty() && rnd_chance((skill_lv * 5 + (status_get_dex(src) + status_get_lv(src)) / 4) - rnd_value(1, 10), 100))
	{
		status_change_end(bl, SC_STONE);
		status_change_end(bl, SC_FREEZE);
		status_change_end(bl, SC_STUN);
		status_change_end(bl, SC_POISON);
		status_change_end(bl, SC_SILENCE);
		status_change_end(bl, SC_BLIND);
		status_change_end(bl, SC_HALLUCINATION);
	}

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
}
break;
case SR_GENTLETOUCH_ENERGYGAIN:
case SR_GENTLETOUCH_CHANGE:
case SR_GENTLETOUCH_REVITALIZE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;
case SR_FLASHCOMBO:
{
	const int32 combo[] = {SR_DRAGONCOMBO, SR_FALLENEMPIRE, SR_TIGERCANNON};
	const int32 delay[] = {0, 750, 1250};

	if (sd) // Disable attacking/acting/moving for skill's duration.
		sd->ud.attackabletime = sd->canuseitem_tick = sd->ud.canact_tick = tick + delay[2];
	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, src, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	for (i = 0; i < ARRAYLENGTH(combo); i++)
		skill_addtimerskill(src, tick + delay[i], bl->id, 0, 0, combo[i], skill_lv, BF_WEAPON, flag | SD_LEVEL);
}
break;

case WA_SWING_DANCE:
case WA_MOONLIT_SERENADE:
case WA_SYMPHONY_OF_LOVER:
case MI_RUSH_WINDMILL:
case MI_ECHOSONG:
	if (!sd || !sd->status.party_id || (flag & 1))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		sc_start2(src, bl, type, 100, skill_lv, ((sd) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON)), skill_get_time(skill_id, skill_lv));
	}
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
		sc_start2(src, bl, type, 100, skill_lv, ((sd) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON)), skill_get_time(skill_id, skill_lv));
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case MI_HARMONIZE:
	if (src != bl)
		clif_skill_nodamage(src, *src, skill_id, skill_lv, sc_start(src, src, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	break;

case WM_DEADHILLHERE:
	if (bl->type == BL_PC)
	{
		if (!status_isdead(*bl))
			break;

		tstatus->hp = max(tstatus->sp, 1);
		tstatus->sp -= tstatus->sp * (60 - 10 * skill_lv) / 100;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		pc_revive(reinterpret_cast<map_session_data *>(bl), true, true);
		clif_resurrection(*bl);
	}
	break;

case WM_VOICEOFSIREN:
	if (flag & 1)
		sc_start2(src, bl, type, skill_area_temp[5], skill_lv, src->id, skill_area_temp[6]);
	else
	{
		// Success chance: (Skill Level x 6) + (Voice Lesson Skill Level x 2) + (Caster's Job Level / 2) %
		skill_area_temp[5] = skill_lv * 6 + ((sd) ? pc_checkskill(sd, WM_LESSON) : 1) * 2 + (sd ? sd->status.job_level : 50) / 2;
		skill_area_temp[6] = skill_get_time(skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ALL | BCT_WOS | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case WM_GLOOMYDAY:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (dstsd && (pc_checkskill(dstsd, KN_BRANDISHSPEAR) || pc_checkskill(dstsd, LK_SPIRALPIERCE) ||
				  pc_checkskill(dstsd, CR_SHIELDCHARGE) || pc_checkskill(dstsd, CR_SHIELDBOOMERANG) ||
				  pc_checkskill(dstsd, PA_SHIELDCHAIN) || pc_checkskill(dstsd, LG_SHIELDPRESS)))
	{ // !TODO: Which skills aren't boosted anymore?
		sc_start(src, bl, SC_GLOOMYDAY_SK, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		break;
	}
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case WM_SATURDAY_NIGHT_FEVER:
	if (flag & 1)
	{
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else if (sd)
	{
		if (rnd() % 100 < sstatus->int_ / 6 + sd->status.job_level / 5 + skill_lv * 4 + pc_checkskill(sd, WM_LESSON))
		{ // !TODO: What's the Lesson bonus?
			map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_PC, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		}
	}
	break;

case WM_SIRCLEOFNATURE:
case WM_SONG_OF_MANA:
case WM_DANCE_WITH_WUG:
case WM_LERADS_DEW:
case WM_UNLIMITED_HUMMING_VOICE:
	if (flag & 1)
	{ // These affect to to all party members near the caster.
		if (sc && sc->getSCE(type))
		{
			sc_start2(src, bl, type, 100, skill_lv, pc_checkskill(sd, WM_LESSON), skill_get_time(skill_id, skill_lv));
		}
	}
	else if (sd)
	{
		if (sc_start2(src, bl, type, 100, skill_lv, pc_checkskill(sd, WM_LESSON), skill_get_time(skill_id, skill_lv)))
			party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case WM_MELODYOFSINK:
	if (flag & 1)
	{
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else
	{ // These affect to all targets around the caster.
		if (rnd() % 100 < 5 + 5 * skill_lv + pc_checkskill(sd, WM_LESSON))
		{ // !TODO: What's the Lesson bonus?
			map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_PC, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		}
	}
	break;

case WM_BEYOND_OF_WARCRY:
	if (flag & 1)
	{
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else
	{ // These affect to all targets around the caster.
		if (rnd() % 100 < 12 + 3 * skill_lv + (sd ? pc_checkskill(sd, WM_LESSON) : 0))
		{ // !TODO: What's the Lesson bonus?
			map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_PC, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		}
	}
	break;

case WM_SOUND_OF_DESTRUCTION:
	if (flag & 1)
	{
		sc_start(src, bl, type, 100, skill_lv, (sd ? pc_checkskill(sd, WM_LESSON) * 500 : 0) + skill_get_time(skill_id, skill_lv)); // !TODO: Confirm Lesson increase
	}
	else
	{
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_PC, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case WM_RANDOMIZESPELL:
	if (rnd() % 100 < 30 + (10 * skill_lv))
	{
		status_change_end(bl, SC_SONGOFMANA);
		status_change_end(bl, SC_DANCEWITHWUG);
		status_change_end(bl, SC_LERADSDEW);
		status_change_end(bl, SC_SATURDAYNIGHTFEVER);
		status_change_end(bl, SC_BEYONDOFWARCRY);
		status_change_end(bl, SC_MELODYOFSINK);
		status_change_end(bl, SC_BEYONDOFWARCRY);
		status_change_end(bl, SC_UNLIMITEDHUMMINGVOICE);

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case RETURN_TO_ELDICASTES:
case ALL_GUARDIAN_RECALL:
case ECLAGE_RECALL:
case ALL_NIFLHEIM_RECALL:
case ALL_PRONTERA_RECALL:
case ALL_GLASTHEIM_RECALL:
case ALL_THANATOS_RECALL:
case ALL_LIGHTHALZEN_RECALL:
	if (sd != nullptr)
	{
		// Destination position.
		uint16 x;
		uint16 y;
		uint16 mapindex;

		switch (skill_id)
		{
		case RETURN_TO_ELDICASTES:
			x = 198;
			y = 187;
			mapindex = mapindex_name2id(MAP_DICASTES);
			break;
		case ALL_GUARDIAN_RECALL:
			x = 44;
			y = 151;
			mapindex = mapindex_name2id(MAP_MORA);
			break;
		case ECLAGE_RECALL:
			x = 47;
			y = 31;
			mapindex = mapindex_name2id(MAP_ECLAGE_IN);
			break;
		case ALL_NIFLHEIM_RECALL:
			x = 193;
			y = 186;
			mapindex = mapindex_name2id(MAP_NIFLHEIM);
			break;
		case ALL_PRONTERA_RECALL:
			if (skill_lv == 1)
			{
				x = 115;
				y = 72;
			}
			else if (skill_lv == 2)
			{
				x = 159;
				y = 192;
			}
			mapindex = mapindex_name2id(MAP_PRONTERA);
			break;
		case ALL_GLASTHEIM_RECALL:
			x = 200;
			y = 268;
			mapindex = mapindex_name2id(MAP_GLASTHEIM);
			break;
		case ALL_THANATOS_RECALL:
			x = 139;
			y = 156;
			mapindex = mapindex_name2id(MAP_THANATOS);
			break;
		case ALL_LIGHTHALZEN_RECALL:
			x = 307;
			y = 307;
			mapindex = mapindex_name2id(MAP_LIGHTHALZEN);
			break;
		}

		sc_start(src, bl, type, 100, skill_lv, skill_get_cooldown(skill_id, skill_lv));

		if (!mapindex)
		{ // Given map not found?
			clif_skill_fail(*sd, skill_id);
			map_freeblock_unlock();
			return 0;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
	break;

case ECL_SNOWFLIP:
case ECL_PEONYMAMY:
case ECL_SADAGUI:
case ECL_SEQUOIADUST:
	switch (skill_id)
	{
	case ECL_SNOWFLIP:
		status_change_end(bl, SC_SLEEP);
		status_change_end(bl, SC_BLEEDING);
		status_change_end(bl, SC_BURNING);
		status_change_end(bl, SC_DEEPSLEEP);
		break;
	case ECL_PEONYMAMY:
		status_change_end(bl, SC_FREEZE);
		status_change_end(bl, SC_FREEZING);
		status_change_end(bl, SC_CRYSTALIZE);
		break;
	case ECL_SADAGUI:
		status_change_end(bl, SC_STUN);
		status_change_end(bl, SC_CONFUSION);
		status_change_end(bl, SC_HALLUCINATION);
		status_change_end(bl, SC_FEAR);
		break;
	case ECL_SEQUOIADUST:
		status_change_end(bl, SC_STONE);
		status_change_end(bl, SC_POISON);
		status_change_end(bl, SC_CURSE);
		status_change_end(bl, SC_BLIND);
		status_change_end(bl, SC_ORCISH);
		status_change_end(bl, SC_DECREASEAGI);
		break;
	}
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, 1, DMG_SINGLE);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	break;

case GM_SANDMAN:
	if (tsc)
	{
		if (tsc->opt1 == OPT1_SLEEP)
			tsc->opt1 = 0;
		else
			tsc->opt1 = OPT1_SLEEP;
		clif_changeoption(bl);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case SO_ARRULLO:
{
	int32 rate = (15 + 5 * skill_lv) + status_get_int(src) / 5 + (sd ? sd->status.job_level / 5 : 0) - status_get_int(bl) / 6 - status_get_luk(bl) / 10;

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	sc_start(src, bl, type, rate, skill_lv, skill_get_time(skill_id, skill_lv));
}
break;

case WM_LULLABY_DEEPSLEEP:
	if (flag & 1)
	{
		int32 rate = 4 * skill_lv + (sd ? pc_checkskill(sd, WM_LESSON) * 2 : 0) + status_get_lv(src) / 15 + (sd ? sd->status.job_level / 5 : 0);
		int32 duration = skill_get_time(skill_id, skill_lv) - (status_get_base_status(bl)->int_ * 50 + status_get_lv(bl) * 50); // Duration reduction for Deep Sleep Lullaby is doubled

		sc_start(src, bl, type, rate, skill_lv, duration);
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
	break;

case SO_SUMMON_AGNI:
case SO_SUMMON_AQUA:
case SO_SUMMON_VENTUS:
case SO_SUMMON_TERA:
	if (sd)
	{
		int32 elemental_class = skill_get_elemental_type(skill_id, skill_lv);

		// Remove previous elemental first.
		if (sd->ed)
			elemental_delete(sd->ed);

		// Summoning the new one.
		if (!elemental_create(sd, elemental_class, skill_get_time(skill_id, skill_lv)))
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case SO_EL_CONTROL:
	if (sd)
	{
		int32 mode;

		if (!sd->ed)
			break;

		if (skill_lv == 4)
		{ // At level 4 delete elementals.
			elemental_delete(sd->ed);
			break;
		}
		switch (skill_lv)
		{ // Select mode bassed on skill level used.
		case 1:
			mode = EL_MODE_PASSIVE;
			break; // Standard mode.
		case 2:
			mode = EL_MODE_ASSIST;
			break;
		case 3:
			mode = EL_MODE_AGGRESSIVE;
			break;
		}
		if (!elemental_change_mode(sd->ed, mode))
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case SO_EL_ACTION:
	if (sd)
	{
		int32 duration = 3000;
		if (!sd->ed)
			break;
		switch (sd->ed->db->class_)
		{
		case ELEMENTALID_AGNI_M:
		case ELEMENTALID_AQUA_M:
		case ELEMENTALID_VENTUS_M:
		case ELEMENTALID_TERA_M:
			duration = 6000;
			break;
		case ELEMENTALID_AGNI_L:
		case ELEMENTALID_AQUA_L:
		case ELEMENTALID_VENTUS_L:
		case ELEMENTALID_TERA_L:
			duration = 9000;
			break;
		}
		sd->skill_id_old = skill_id;
		elemental_action(sd->ed, bl, tick);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		skill_blockpc_start(*sd, skill_id, duration);
	}
	break;

case SO_EL_CURE:
	if (sd)
	{
		s_elemental_data *ed = sd->ed;
		int32 s_hp, s_sp;

		if (!ed)
			break;

		s_hp = sd->battle_status.hp * 10 / 100;
		s_sp = sd->battle_status.sp * 10 / 100;

		if (!status_charge(sd, s_hp, s_sp))
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}

		status_heal(ed, s_hp, s_sp, 3);
		clif_skill_nodamage(src, *ed, skill_id, skill_lv);
	}
	break;

case GN_CHANGEMATERIAL:
case SO_EL_ANALYSIS:
	if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		clif_skill_itemlistwindow(sd, skill_id, skill_lv);
	}
	break;

case NPC_MANDRAGORA:
case GN_MANDRAGORA:
	if (flag & 1)
	{
		int32 rate;
		if (skill_id == NPC_MANDRAGORA)
			rate = (20 * skill_lv) - (tstatus->vit + tstatus->luk) / 5;
		else
			rate = 25 + (10 * skill_lv) - (tstatus->vit + tstatus->luk) / 5;

		if (rate < 10)
			rate = 10;
		if (bl->type == BL_MOB || (tsc && tsc->getSCE(type)))
			break; // Don't activate if target is a monster or zap SP if target already has Mandragora active.
		if (rnd() % 100 < rate)
		{
			sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
			status_zap(bl, 0, status_get_max_sp(bl) * (25 + 5 * skill_lv) / 100);
		}
	}
	else
	{
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *src, skill_id, skill_lv);
	}
	break;
case GN_SLINGITEM:
	if (sd)
	{
		i = sd->equip_index[EQI_AMMO];
		if (i < 0)
			break; // No ammo.
		t_itemid ammo_id = sd->inventory_data[i]->nameid;
		if (ammo_id == 0)
			break;
		sd->itemid = ammo_id;
		if (itemdb_group.item_exists(IG_BOMB, ammo_id))
		{
			if (battle_check_target(src, bl, BCT_ENEMY) > 0)
			{ // Only attack if the target is an enemy.
				if (ammo_id == ITEMID_PINEAPPLE_BOMB)
					map_foreachincell(skill_area_sub, bl->m, bl->x, bl->y, BL_CHAR, src, GN_SLINGITEM_RANGEMELEEATK, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
				else
					skill_attack(BF_WEAPON, src, src, bl, GN_SLINGITEM_RANGEMELEEATK, skill_lv, tick, flag);
			}
			else // Otherwise, it fails, shows animation and removes items.
				clif_skill_fail(*sd, GN_SLINGITEM_RANGEMELEEATK, USESKILL_FAIL);
		}
		else if (itemdb_group.item_exists(IG_THROWABLE, ammo_id))
		{
			switch (ammo_id)
			{
			case ITEMID_HP_INC_POTS_TO_THROW: // MaxHP +(500 + Thrower BaseLv * 10 / 3) and heals 1% MaxHP
				sc_start2(src, bl, SC_PROMOTE_HEALTH_RESERCH, 100, 2, 1, 500000);
				status_percent_heal(bl, 1, 0);
				break;
			case ITEMID_HP_INC_POTM_TO_THROW: // MaxHP +(1500 + Thrower BaseLv * 10 / 3) and heals 2% MaxHP
				sc_start2(src, bl, SC_PROMOTE_HEALTH_RESERCH, 100, 2, 2, 500000);
				status_percent_heal(bl, 2, 0);
				break;
			case ITEMID_HP_INC_POTL_TO_THROW: // MaxHP +(2500 + Thrower BaseLv * 10 / 3) and heals 5% MaxHP
				sc_start2(src, bl, SC_PROMOTE_HEALTH_RESERCH, 100, 2, 3, 500000);
				status_percent_heal(bl, 5, 0);
				break;
			case ITEMID_SP_INC_POTS_TO_THROW: // MaxSP +(Thrower BaseLv / 10 - 5)% and recovers 2% MaxSP
				sc_start2(src, bl, SC_ENERGY_DRINK_RESERCH, 100, 2, 1, 500000);
				status_percent_heal(bl, 0, 2);
				break;
			case ITEMID_SP_INC_POTM_TO_THROW: // MaxSP +(Thrower BaseLv / 10)% and recovers 4% MaxSP
				sc_start2(src, bl, SC_ENERGY_DRINK_RESERCH, 100, 2, 2, 500000);
				status_percent_heal(bl, 0, 4);
				break;
			case ITEMID_SP_INC_POTL_TO_THROW: // MaxSP +(Thrower BaseLv / 10 + 5)% and recovers 8% MaxSP
				sc_start2(src, bl, SC_ENERGY_DRINK_RESERCH, 100, 2, 3, 500000);
				status_percent_heal(bl, 0, 8);
				break;
			default:
				if (dstsd)
					run_script(sd->inventory_data[i]->script, 0, dstsd->id, fake_nd->id);
				break;
			}
		}
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv); // This packet is received twice actually, I think it is to show the animation.
	break;
case GN_MIX_COOKING:
case GN_MAKEBOMB:
case GN_S_PHARMACY:
	if (sd)
	{
		int32 qty = 1;
		sd->skill_id_old = skill_id;
		sd->skill_lv_old = skill_lv;
		if (skill_id != GN_S_PHARMACY && skill_lv > 1)
			qty = 10;
		clif_cooking_list(*sd, (skill_id - GN_MIX_COOKING) + 27, skill_id, qty, skill_id == GN_MAKEBOMB ? 5 : 6);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case EL_CIRCLE_OF_FIRE:
case EL_PYROTECHNIC:
case EL_HEATER:
case EL_TROPIC:
case EL_AQUAPLAY:
case EL_COOLER:
case EL_CHILLY_AIR:
case EL_GUST:
case EL_BLAST:
case EL_WILD_STORM:
case EL_PETROLOGY:
case EL_CURSED_SOIL:
case EL_UPHEAVAL:
case EL_FIRE_CLOAK:
case EL_WATER_DROP:
case EL_WIND_CURTAIN:
case EL_SOLID_SKIN:
case EL_STONE_SHIELD:
case EL_WIND_STEP:
case EM_EL_FLAMETECHNIC:
case EM_EL_FLAMEARMOR:
case EM_EL_COLD_FORCE:
case EM_EL_CRYSTAL_ARMOR:
case EM_EL_GRACE_BREEZE:
case EM_EL_EYES_OF_STORM:
case EM_EL_EARTH_CARE:
case EM_EL_STRONG_PROTECTION:
case EM_EL_DEEP_POISONING:
case EM_EL_POISON_SHIELD:
{
	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if (ele)
	{
		sc_type type2 = (sc_type)(type - 1);
		status_change *esc = status_get_sc(ele);

		if ((esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)))
		{
			status_change_end(src, type);
			status_change_end(bl, type2);
		}
		else
		{
			clif_skill_nodamage(src, *src, skill_id, skill_lv);
			if (!(skill_id >= EM_EL_FLAMETECHNIC && skill_id <= EM_EL_DEADLY_POISON))
				clif_skill_damage(*src, (skill_id == EL_GUST || skill_id == EL_BLAST || skill_id == EL_WILD_STORM) ? *src : *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
			if (skill_id == EL_WIND_STEP) // There aren't teleport, just push the master away.
				skill_blown(src, bl, (rnd() % skill_get_blewcount(skill_id, skill_lv)) + 1, rnd() % 8, BLOWN_NONE);
			sc_start(src, src, type2, 100, skill_lv, skill_get_time(skill_id, skill_lv));
			sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		}
	}
}
break;
case EL_FIRE_MANTLE:
case EL_WATER_BARRIER:
case EL_ZEPHYR:
case EL_POWER_OF_GAIA:
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	skill_unitsetting(src, skill_id, skill_lv, bl->x, bl->y, 0);
	break;
case EL_WATER_SCREEN:
{
	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if (ele)
	{
		status_change *esc = status_get_sc(ele);
		sc_type type2 = (sc_type)(type - 1);

		clif_skill_nodamage(src, *src, skill_id, skill_lv);
		if ((esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)))
		{
			status_change_end(bl, type);
			status_change_end(src, type2);
		}
		else
		{
			// This not heals at the end.
			clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
			sc_start(src, src, type2, 100, skill_lv, skill_get_time(skill_id, skill_lv));
			sc_start(src, bl, type, 100, src->id, skill_get_time(skill_id, skill_lv));
		}
	}
}
break;

case KO_KAHU_ENTEN:
case KO_HYOUHU_HUBUKI:
case KO_KAZEHU_SEIRAN:
case KO_DOHU_KOUKAI:
	if (sd)
	{
		int32 ele_type = skill_get_ele(skill_id, skill_lv);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		pc_addspiritcharm(sd, skill_get_time(skill_id, skill_lv), MAX_SPIRITCHARM, ele_type);
	}
	break;
case KO_ZANZOU:
	if (sd)
	{
		struct mob_data *md2;

		md2 = mob_once_spawn_sub(src, src->m, src->x, src->y, status_get_name(*src), MOBID_ZANZOU, "", SZ_SMALL, AI_NONE);
		if (md2)
		{
			md2->master_id = src->id;
			md2->special_state.ai = AI_ZANZOU;
			if (md2->deletetimer != INVALID_TIMER)
				delete_timer(md2->deletetimer, mob_timer_delete);
			md2->deletetimer = add_timer(gettick() + skill_get_time(skill_id, skill_lv), mob_timer_delete, md2->id, 0);
			mob_spawn(md2);
			map_foreachinallrange(unit_changetarget, src, AREA_SIZE, BL_MOB, src, md2);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			skill_blown(src, bl, skill_get_blewcount(skill_id, skill_lv), unit_getdir(bl), BLOWN_NONE);
		}
	}
	break;

case KO_KYOUGAKU:
	if (dstsd && tsc && !tsc->getSCE(type) && rnd() % 100 < tstatus->int_ / 2)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv,
							sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
	break;
case KO_JYUSATSU:
	if (dstsd && tsc && !tsc->getSCE(type) &&
		rnd() % 100 < ((45 + 5 * skill_lv) + skill_lv * 5 - status_get_int(bl) / 2))
	{ //[(Base chance of success) + (Skill Level x 5) - (int32 / 2)]%.
		clif_skill_nodamage(src, *bl, skill_id, skill_lv,
							status_change_start(src, bl, type, 10000, skill_lv, 0, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF));
		status_percent_damage(src, bl, tstatus->hp * skill_lv * 5, 0, false); // Does not kill the target.
		if (status_get_lv(bl) <= status_get_lv(src))
			status_change_start(src, bl, SC_COMA, 10, skill_lv, 0, src->id, 0, 0, SCSTART_NONE);
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
	break;
case KO_GENWAKU:
	if ((dstsd || dstmd) && !status_has_mode(tstatus, MD_IGNOREMELEE | MD_IGNOREMAGIC | MD_IGNORERANGED | MD_IGNOREMISC) && battle_check_target(src, bl, BCT_ENEMY) > 0)
	{
		int32 x = src->x, y = src->y;

		if (sd && rnd() % 100 > ((45 + 5 * skill_lv) - status_get_int(bl) / 10))
		{ //[(Base chance of success) - (Intelligence Objectives / 10)]%.
			clif_skill_fail(*sd, skill_id);
			break;
		}

		// Confusion is still inflicted (but rate isn't reduced), no matter map type.
		status_change_start(src, src, SC_CONFUSION, 2500, skill_lv, 0, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NORATEDEF);
		status_change_start(src, bl, SC_CONFUSION, 7500, skill_lv, 0, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NORATEDEF);

		if (skill_check_unit_movepos(5, src, bl->x, bl->y, 0, 0))
		{
			clif_skill_nodamage(src, *src, skill_id, skill_lv);
			clif_blown(src);
			if (!unit_blown_immune(bl, 0x1))
			{
				unit_movepos(bl, x, y, 0, 0);
				if (bl->type == BL_PC && pc_issit((TBL_PC *)bl))
					clif_sitting(*bl); // Avoid sitting sync problem
				clif_blown(bl);
				map_foreachinallrange(unit_changetarget, src, AREA_SIZE, BL_CHAR, src, bl);
			}
		}
	}
	break;
case OB_AKAITSUKI:
case OB_OBOROGENSOU:
	if (sd && ((skill_id == OB_OBOROGENSOU && bl->type == BL_MOB) // This skill does not work on monsters.
			   || status_bl_has_mode(bl, MD_STATUSIMMUNE)))
	{ // Does not work on status immune monsters.
		clif_skill_fail(*sd, skill_id);
		break;
	}
	[[fallthrough]];
case KO_IZAYOI:
case OB_ZANGETSU:
case KG_KYOMU:
case KG_KAGEMUSYA:
case SP_SOULDIVISION:
	if (skill_id == SP_SOULDIVISION)
	{ // Usable only on other players.
		if (bl->type != BL_PC)
		{
			if (sd)
				clif_skill_fail(*sd, skill_id);
			break;
		}
	}

	clif_skill_nodamage(src, *bl, skill_id, skill_lv,
						sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	break;
case KG_KAGEHUMI:
	if (flag & 1)
	{
		if (bl->type != BL_PC)
			break;
		if (tsc && (tsc->option & (OPTION_CLOAK | OPTION_HIDE) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC__SHADOWFORM) || tsc->getSCE(SC_MARIONETTE) || tsc->getSCE(SC_HARMONIZE)))
		{
			status_change_end(bl, SC_HIDING);
			status_change_end(bl, SC_CLOAKING);
			status_change_end(bl, SC_CLOAKINGEXCEED);
			status_change_end(bl, SC_CAMOUFLAGE);
			status_change_end(bl, SC_NEWMOON);
			if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
				status_change_end(bl, SC__SHADOWFORM);
			status_change_end(bl, SC_MARIONETTE);
			status_change_end(bl, SC_HARMONIZE);
			sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		}
	}
	else
	{
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case MH_SILENT_BREEZE:
{
	int32 heal = 5 * status_get_lv(hd) +
#ifdef RENEWAL
				 status_base_matk_min(bl, &hd->battle_status, status_get_lv(hd));
#else
				 status_base_matk_min(&hd->battle_status);
#endif
	// Silences the homunculus and target
	status_change_start(src, src, SC_SILENCE, 10000, skill_lv, 0, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NONE);
	status_change_start(src, bl, SC_SILENCE, 10000, skill_lv, 0, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NONE);

	// Recover the target's HP
	status_heal(bl, heal, 0, 3);

	// Removes these SC from target
	if (tsc)
	{
		const enum sc_type scs[] = {
			SC_MANDRAGORA, SC_HARMONIZE, SC_DEEPSLEEP, SC_VOICEOFSIREN, SC_SLEEP, SC_CONFUSION, SC_HALLUCINATION};
		for (i = 0; i < ARRAYLENGTH(scs); i++)
			if (tsc->getSCE(scs[i]))
				status_change_end(bl, scs[i]);
	}
}
break;
case MH_OVERED_BOOST:
	if (hd && battle_get_master(src))
	{
		sc_start(src, battle_get_master(src), type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	break;
case MH_GRANITIC_ARMOR:
case MH_PYROCLASTIC:
	if (hd)
	{
		struct block_list *s_bl = battle_get_master(src);
		if (s_bl)
			sc_start2(src, s_bl, type, 100, skill_lv, hd->homunculus.level, skill_get_time(skill_id, skill_lv)); // start on master
		sc_start2(src, bl, type, 100, skill_lv, hd->homunculus.level, skill_get_time(skill_id, skill_lv));
	}
	break;
case MH_LIGHT_OF_REGENE: // self
	if (hd)
	{
		struct block_list *s_bl = battle_get_master(src);
		if (s_bl)
			sc_start(src, s_bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		sc_start2(src, src, type, 100, skill_lv, hd->homunculus.level, skill_get_time(skill_id, skill_lv));
	}
	break;
case MH_STYLE_CHANGE:
	if (hd)
	{
		struct status_change_entry *sce;
		if ((sce = hd->sc.getSCE(SC_STYLE_CHANGE)) != nullptr)
		{ // in preparation for other bl usage
			if (sce->val1 == MH_MD_FIGHTING)
				sce->val1 = MH_MD_GRAPPLING;
			else
				sce->val1 = MH_MD_FIGHTING;
			// if(hd->master && hd->sc.getSCE(SC_STYLE_CHANGE)) { // Aegis does not show any message when switching fighting style
			//	char output[128];
			//	safesnprintf(output,sizeof(output),msg_txt(sd,378),(sce->val1==MH_MD_FIGHTING?"fighthing":"grappling"));
			//	clif_messagecolor(hd->master, color_table[COLOR_RED], output, false, SELF);
			// }
		}
		else
			sc_start(hd, hd, SC_STYLE_CHANGE, 100, MH_MD_FIGHTING, INFINITE_TICK);
	}
	break;
case MH_GOLDENE_TONE:
case MH_TEMPERING:
{
	block_list *master_bl = battle_get_master(src);

	if (master_bl != nullptr)
	{
		clif_skill_nodamage(src, *master_bl, skill_id, skill_lv);
		sc_start(src, master_bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
}
break;
case MH_PAIN_KILLER:
	bl = battle_get_master(src);
	if (bl != nullptr)
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case MH_MAGMA_FLOW:
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case MH_SUMMON_LEGION:
{
	int32 summons[5] = {MOBID_S_HORNET, MOBID_S_GIANT_HORNET, MOBID_S_GIANT_HORNET, MOBID_S_LUCIOLA_VESPA, MOBID_S_LUCIOLA_VESPA};
	int32 qty[5] = {3, 3, 4, 4, 5};
	struct mob_data *sum_md;
	int32 i_slave, c = 0;

	int32 maxcount = qty[skill_lv - 1];
	i_slave = map_foreachinmap(skill_check_condition_mob_master_sub, hd->m, BL_MOB, hd->id, summons[skill_lv - 1], skill_id, &c);
	if (c >= maxcount)
	{
		map_freeblock_unlock();
		return 0; // max qty already spawned
	}

	for (i_slave = 0; i_slave < qty[skill_lv - 1]; i_slave++)
	{ // easy way
		sum_md = mob_once_spawn_sub(src, src->m, src->x, src->y, status_get_name(*src), summons[skill_lv - 1], "", SZ_SMALL, AI_ATTACK);
		if (sum_md)
		{
			sum_md->master_id = src->id;
			sum_md->special_state.ai = AI_LEGION;
			if (sum_md->deletetimer != INVALID_TIMER)
				delete_timer(sum_md->deletetimer, mob_timer_delete);
			sum_md->deletetimer = add_timer(gettick() + skill_get_time(skill_id, skill_lv), mob_timer_delete, sum_md->id, 0);
			mob_spawn(sum_md); // Now it is ready for spawning.
			sc_start4(sum_md, sum_md, SC_MODECHANGE, 100, 1, 0, MD_CANATTACK | MD_AGGRESSIVE, 0, 60000);
		}
	}
}
break;

case RL_RICHS_COIN:
	if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		for (i = 0; i < 10; i++)
			pc_addspiritball(sd, skill_get_time(skill_id, skill_lv), 10);
	}
	break;
case RL_C_MARKER:
	if (sd)
	{
		// If marked by someone else remove it
		if (tsce && tsce->val2 != src->id)
			status_change_end(bl, type);

		// Check if marked before
		ARR_FIND(0, MAX_SKILL_CRIMSON_MARKER, i, sd->c_marker[i] == bl->id);
		if (i == MAX_SKILL_CRIMSON_MARKER)
		{
			// Find empty slot
			ARR_FIND(0, MAX_SKILL_CRIMSON_MARKER, i, !sd->c_marker[i]);
			if (i == MAX_SKILL_CRIMSON_MARKER)
			{
				clif_skill_fail(*sd, skill_id);
				break;
			}
		}

		sd->c_marker[i] = bl->id;
		status_change_start(src, bl, type, 10000, skill_lv, src->id, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	// If mob casts this, at least SC_C_MARKER as debuff
	else
	{
		status_change_start(src, bl, type, 10000, skill_lv, src->id, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;
case RL_QD_SHOT:
	// Remember main target as it will always be hit by this skill
	skill_area_temp[1] = bl->id;
	// Iterate through all enemies in the area
	map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	// End here to prevent spamming of the skill onto the target
	status_change_end(src, SC_QD_SHOT_READY);
	skill_area_temp[1] = 0;
	break;
case RL_FLICKER:
	if (sd)
	{
		sd->flicker = true;
		skill_area_temp[1] = 0;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		// Detonate RL_B_TRAP
		if (pc_checkskill(sd, RL_B_TRAP))
			map_foreachinallrange(skill_bind_trap, src, AREA_SIZE, BL_SKILL, src);
		// Detonate RL_H_MINE
		if ((i = pc_checkskill(sd, RL_H_MINE)))
			map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, RL_H_MINE, i, tick, flag | BCT_ENEMY | SD_SPLASH, skill_castend_damage_id);
		sd->flicker = false;
	}
	break;

case SO_ELEMENTAL_SHIELD:
	if (!sd || sd->status.party_id == 0 || flag & 1)
	{
		if (sd && sd->status.party_id == 0)
		{
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			if (sd->ed && skill_get_state(skill_id) == ST_ELEMENTALSPIRIT2)
				elemental_delete(sd->ed);
		}
		skill_unitsetting(bl, MG_SAFETYWALL, skill_lv + 5, bl->x, bl->y, 0);
		skill_unitsetting(bl, AL_PNEUMA, 1, bl->x, bl->y, 0);
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		if (sd->ed && skill_get_state(skill_id) == ST_ELEMENTALSPIRIT2)
			elemental_delete(sd->ed);
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
	break;

case SU_HIDE:
	if (tsce)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		status_change_end(bl, type);
		map_freeblock_unlock();
		return 0;
	}
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case SU_STOOP:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case SU_SV_ROOTTWIST:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (sd && status_get_class_(bl) == CLASS_BOSS)
	{
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL_TOTARGET);
		break;
	}
	if (tsc != nullptr && tsc->getSCE(type)) // Refresh the status only if it's already active.
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	else
	{
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		if (sd && pc_checkskill(sd, SU_SPIRITOFLAND))
			sc_start(src, src, SC_DORAM_MATK, 100, sd->status.base_level, skill_get_time(SU_SPIRITOFLAND, 1));
		skill_addtimerskill(src, tick + 1000, bl->id, 0, 0, SU_SV_ROOTTWIST_ATK, skill_lv, skill_get_type(SU_SV_ROOTTWIST_ATK), flag);
	}
	break;

case SU_TUNABELLY:
{
	uint32 heal = 0;

	if (dstmd && (dstmd->mob_id == MOBID_EMPERIUM || status_get_class_(bl) == CLASS_BATTLEFIELD))
		heal = 0;
	else if (status_get_hp(bl) != status_get_max_hp(bl))
		heal = ((2 * skill_lv - 1) * 10) * status_get_max_hp(bl) / 100;
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	status_heal(bl, heal, 0, 0);
}
break;

case SU_BUNCHOFSHRIMP:
case SU_HISS:
case SU_PURRING:
case SU_MEOWMEOW:
	if (sd == nullptr || sd->status.party_id == 0 || flag & 1)
	{
		int32 duration = skill_get_time(skill_id, skill_lv);

		if (skill_id == SU_BUNCHOFSHRIMP && pc_checkskill(sd, SU_SPIRITOFSEA))
			duration += skill_get_time2(SU_BUNCHOFSHRIMP, skill_lv);
		clif_skill_nodamage(bl, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, duration));
	}
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
	break;

case SU_SHRIMPARTY:
	if (sd == nullptr || sd->status.party_id == 0 || flag & 1)
	{
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		if (sd && (i = pc_checkskill(sd, SU_FRESHSHRIMP)) > 0)
		{
			clif_skill_nodamage(bl, *bl, SU_FRESHSHRIMP, i, 1);
			sc_start(src, bl, SC_FRESHSHRIMP, 100, i, skill_get_time(SU_FRESHSHRIMP, i));
		}
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	break;

case SU_POWEROFFLOCK:
	if (flag & 1)
	{
		sc_start(src, bl, SC_FEAR, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		sc_start(src, bl, SC_FREEZE, 100, skill_lv, skill_get_time2(skill_id, skill_lv)); //! TODO: What's the duration?
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		if (battle_config.skill_wall_check)
			map_foreachinshootrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		else
			map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
	break;

case ALL_EQSWITCH:
	if (sd)
	{
		clif_equipswitch_reply(sd, false);

		for (int32 i = 0, position = 0; i < EQI_MAX; i++)
		{
			if (sd->equip_switch_index[i] >= 0 && !(position & equip_bitmask[i]))
			{
				position |= pc_equipswitch(sd, sd->equip_switch_index[i]);
			}
		}
	}
	break;

case AB_VITUPERATUM:
	if (flag & 1)
		clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
	else
	{
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case AB_CONVENIO:
	if (sd)
	{
		party_data *p = party_search(sd->status.party_id);
		int32 i = 0, count = 0;

		// Only usable in party
		if (p == nullptr)
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}

		// Only usable as party leader.
		ARR_FIND(0, MAX_PARTY, i, p->data[i].sd == sd);
		if (i == MAX_PARTY || !p->party.member[i].leader)
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}

		// Do the teleport part
		for (i = 0; i < MAX_PARTY; ++i)
		{
			map_session_data *pl_sd = p->data[i].sd;

			if (pl_sd == nullptr || pl_sd == sd || pl_sd->status.party_id != p->party.party_id || pc_isdead(pl_sd) ||
				sd->m != pl_sd->m)
				continue;

			// Respect /call configuration
			if (pl_sd->status.disable_call)
			{
				continue;
			}

			if (!(map_getmapflag(sd->m, MF_NOTELEPORT) || map_getmapflag(sd->m, MF_PVP) || map_getmapflag(sd->m, MF_BATTLEGROUND) || map_flag_gvg2(sd->m)))
			{
				pc_setpos(pl_sd, map_id2index(sd->m), sd->x, sd->y, CLR_TELEPORT);
				count++;
			}
		}
		if (!count)
			clif_skill_fail(*sd, skill_id);
	}
	break;

case NPC_PULSESTRIKE2:
	for (int32 i = 0; i < 3; i++)
		skill_addtimerskill(src, tick + (t_tick)skill_get_time(skill_id, skill_lv) * i, bl->id, 0, 0, skill_id, skill_lv, skill_get_type(skill_id), flag);
	break;

case BO_THE_WHOLE_PROTECTION:
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
	{
		uint32 equip[] = {EQP_WEAPON, EQP_SHIELD, EQP_ARMOR, EQP_HEAD_TOP};

		for (uint8 i_eqp = 0; i_eqp < 4; i_eqp++)
		{
			if (bl->type != BL_PC || (dstsd && pc_checkequip(dstsd, equip[i_eqp]) < 0))
				continue;
			sc_start(src, bl, (sc_type)(SC_CP_WEAPON + i_eqp), 100, skill_lv, skill_get_time(skill_id, skill_lv));
		}
	}
	else if (sd)
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
	break;

case TR_MUSICAL_INTERLUDE:
case TR_JAWAII_SERENADE:
case TR_PRON_MARCH:
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
		sc_start4(src, bl, type, 100, skill_lv, 0, flag, 0, skill_get_time(skill_id, skill_lv));
	else if (sd)
	{
		clif_skill_nodamage(bl, *bl, skill_id, skill_lv);

		sd->skill_id_song = skill_id;
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, skill_id, &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(skill_id, skill_lv), src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
	break;

case TR_GEF_NOCTURN:
case TR_AIN_RHAPSODY:
	if (flag & 1)
		sc_start4(src, bl, type, 100, skill_lv, 0, flag, 0, skill_get_time(skill_id, skill_lv));
	else if (sd)
	{
		clif_skill_nodamage(bl, *bl, skill_id, skill_lv);

		sd->skill_id_song = skill_id;
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, skill_id, &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
	break;

case TR_ROKI_CAPRICCIO:
case TR_NIPELHEIM_REQUIEM:
	if (flag & 1)
	{ // Need official success chances.
		uint16 success_chance = 5 * skill_lv;

		if (flag & 2)
			success_chance *= 2;

		// Is it a chance to inflect so and so, or seprate chances for inflicting each status? [Rytech]
		if (skill_id == TR_ROKI_CAPRICCIO)
		{
			sc_start(src, bl, SC_CONFUSION, 4 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
			sc_start(src, bl, SC_HANDICAPSTATE_MISFORTUNE, success_chance, skill_lv, skill_get_time2(skill_id, skill_lv));
		}
		else
		{ // TR_NIPELHEIM_REQUIEM
			sc_start(src, bl, SC_CURSE, 4 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
			sc_start(src, bl, SC_HANDICAPSTATE_DEPRESSION, success_chance, skill_lv, skill_get_time2(skill_id, skill_lv));
		}
	}
	else if (sd)
	{
		clif_skill_nodamage(bl, *bl, skill_id, skill_lv);

		sd->skill_id_song = skill_id;
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, skill_id, &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		map_foreachinallrange(skill_area_sub, src, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
	break;

case ABR_NET_REPAIR:
case ABR_NET_SUPPORT:
	if (flag & 1)
	{
		int32 heal_amount;

		if (skill_id == ABR_NET_REPAIR)
		{
			heal_amount = tstatus->max_hp * 10 / 100;
			clif_skill_nodamage(nullptr, *bl, AL_HEAL, heal_amount);
			status_heal(bl, heal_amount, 0, 0);
		}
		else
		{ // ABR_NET_SUPPORT
			heal_amount = tstatus->max_sp * 3 / 100;
			clif_skill_nodamage(nullptr, *bl, MG_SRECOVERY, heal_amount);
			status_heal(bl, 0, heal_amount, 0);
		}
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ALLY | SD_SPLASH | 1, skill_castend_nodamage_id);
	}
	break;

case WH_HAWK_M:
	if (sd)
	{
		if (!pc_isfalcon(sd))
			pc_setoption(sd, sd->sc.option | OPTION_FALCON);
		else
			pc_setoption(sd, sd->sc.option & ~OPTION_FALCON);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case EM_SUMMON_ELEMENTAL_ARDOR:
case EM_SUMMON_ELEMENTAL_DILUVIO:
case EM_SUMMON_ELEMENTAL_PROCELLA:
case EM_SUMMON_ELEMENTAL_TERREMOTUS:
case EM_SUMMON_ELEMENTAL_SERPENS:
{
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);

	if (sd == nullptr)
		break;

	uint16 em_elem[5] = {ELEMENTALID_ARDOR, ELEMENTALID_DILUVIO, ELEMENTALID_PROCELLA, ELEMENTALID_TERREMOTUS, ELEMENTALID_SERPENS};
	uint16 so_elem[5] = {ELEMENTALID_AGNI_L, ELEMENTALID_AQUA_L, ELEMENTALID_VENTUS_L, ELEMENTALID_TERA_L, 0};
	uint8 elem_value = 4 - (EM_SUMMON_ELEMENTAL_SERPENS - skill_id);

	if (sd->ed && ((skill_id >= EM_SUMMON_ELEMENTAL_ARDOR && skill_id <= EM_SUMMON_ELEMENTAL_TERREMOTUS && sd->ed->elemental.class_ == so_elem[elem_value]) ||
				   (skill_id == EM_SUMMON_ELEMENTAL_SERPENS &&
					(sd->ed->elemental.class_ == ELEMENTALID_AGNI_L || sd->ed->elemental.class_ == ELEMENTALID_AQUA_L ||
					 sd->ed->elemental.class_ == ELEMENTALID_VENTUS_L || sd->ed->elemental.class_ == ELEMENTALID_TERA_L))))
	{
		// Remove the old elemental before summoning the super one.
		elemental_delete(sd->ed);

		if (!elemental_create(sd, em_elem[elem_value], skill_get_time(skill_id, skill_lv)))
		{
			clif_skill_fail(*sd, skill_id);
			break;
		}
		else // Elemental summoned. Buff the player with the bonus.
			sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else
	{
		clif_skill_fail(*sd, skill_id);
		break;
	}
}
break;

case EM_ELEMENTAL_VEIL:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);

	if (sd == nullptr)
		break;

	if (sd->ed && sd->ed->elemental.class_ >= ELEMENTALID_DILUVIO && sd->ed->elemental.class_ <= ELEMENTALID_SERPENS)
		sc_start(src, sd->ed, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	else
		clif_skill_fail(*sd, skill_id);
	break;

case MT_M_MACHINE:
case BO_BIONIC_PHARMACY:
	if (sd)
	{
		sd->skill_id_old = skill_id;
		sd->skill_lv_old = skill_lv;

		if (skill_id == MT_M_MACHINE)
			clif_cooking_list(*sd, 31, skill_id, 1, 7);
		else // BO_BIONIC_PHARMACY
			clif_cooking_list(*sd, 32, skill_id, 1, 8);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	break;

case MT_SUMMON_ABR_BATTLE_WARIOR:
case MT_SUMMON_ABR_DUAL_CANNON:
case MT_SUMMON_ABR_MOTHER_NET:
case MT_SUMMON_ABR_INFINITY:
{
	uint32 abrs[4] = {MOBID_ABR_BATTLE_WARIOR, MOBID_ABR_DUAL_CANNON, MOBID_ABR_MOTHER_NET, MOBID_ABR_INFINITY};

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", abrs[3 - (MT_SUMMON_ABR_INFINITY - skill_id)], "", SZ_SMALL, AI_ABR);

	if (md)
	{
		md->master_id = src->id;
		md->special_state.ai = AI_ABR;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(skill_id, skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}
break;

case BO_WOODENWARRIOR:
case BO_WOODEN_FAIRY:
case BO_CREEPER:
case BO_HELLTREE:
{ // A poring is used in the 4th slot as a dummy since the Research Report skill is in between the Creeper and Hell Tree skills.
	uint32 bionics[5] = {MOBID_BIONIC_WOODENWARRIOR, MOBID_BIONIC_WOODEN_FAIRY, MOBID_BIONIC_CREEPER, MOBID_PORING, MOBID_BIONIC_HELLTREE};

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", bionics[4 - (BO_HELLTREE - skill_id)], "", SZ_SMALL, AI_BIONIC);

	if (md)
	{
		md->master_id = src->id;
		md->special_state.ai = AI_BIONIC;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(skill_id, skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}
break;

#ifdef RENEWAL
case CG_HERMODE:
	skill_castend_song(src, skill_id, skill_lv, tick);
	break;
#endif

case NPC_LEASH:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);

	if (!skill_check_unit_movepos(0, bl, src->x, src->y, 1, 1))
	{
		return 0;
	}

	clif_blown(bl);
	break;

case NPC_WIDELEASH:
	if (flag & 1)
	{
		if (!skill_check_unit_movepos(0, bl, src->x, src->y, 1, 1))
		{
			return 0;
		}

		clif_blown(bl);
	}
	else
	{
		skill_area_temp[2] = 0; // For SD_PREAMBLE
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_PREAMBLE | 1, skill_castend_nodamage_id);
	}
	break;

case HN_HELLS_DRIVE:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	break;

case NW_THE_VIGILANTE_AT_NIGHT:
	i = skill_get_splash(skill_id, skill_lv);
	skill_area_temp[0] = 0;
	skill_area_temp[1] = bl->id;
	skill_area_temp[2] = 0;

	if (sd && sd->weapontype1 == W_GATLING)
	{
		i = 5; // 11x11
		clif_skill_nodamage(src, *bl, NW_THE_VIGILANTE_AT_NIGHT_GUN_GATLING, skill_lv);
	}
	else
		clif_skill_nodamage(src, *bl, NW_THE_VIGILANTE_AT_NIGHT_GUN_SHOTGUN, skill_lv);
	map_foreachinrange(skill_area_sub, bl, i, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
	break;

case NW_INTENSIVE_AIM:
	if (tsc && tsc->getSCE(type))
	{
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
		status_change_end(bl, type);
	}
	else
	{
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
	break;

case NW_HIDDEN_CARD:
case NW_AUTO_FIRING_LAUNCHER:
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
	break;

case NW_GRENADE_FRAGMENT:
	status_change_end(src, type);
	if (skill_lv < 7)
		sc_start(src, bl, (sc_type)(SC_GRENADE_FRAGMENT_1 - 1 + skill_lv), 100, skill_lv, skill_get_time(skill_id, skill_lv));
	else if (skill_lv == 7)
	{
		status_change_end(src, SC_GRENADE_FRAGMENT_1);
		status_change_end(src, SC_GRENADE_FRAGMENT_2);
		status_change_end(src, SC_GRENADE_FRAGMENT_3);
		status_change_end(src, SC_GRENADE_FRAGMENT_4);
		status_change_end(src, SC_GRENADE_FRAGMENT_5);
		status_change_end(src, SC_GRENADE_FRAGMENT_6);
	}
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
	break;
case ALL_EVENT_20TH_ANNIVERSARY:
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
	break;

case SOA_SOUL_GATHERING:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);

	if (sd != nullptr)
	{
		int32 limit = 5 + pc_checkskill(sd, SP_SOULENERGY) * 3;

		for (i = 0; i < limit; i++)
			pc_addsoulball(*sd, limit);
	}
	break;

case SOA_TALISMAN_OF_WARRIOR:
case SOA_TALISMAN_OF_MAGICIAN:
case SOA_TALISMAN_OF_FIVE_ELEMENTS:
	if (dstsd != nullptr)
	{
		int16 index = dstsd->equip_index[EQI_HAND_R];

		if (index >= 0 && dstsd->inventory_data[index] != nullptr && dstsd->inventory_data[index]->type == IT_WEAPON)
		{
			clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv)));
			break;
		}
	}

	if (sd != nullptr)
	{
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL_NEED_WEAPON);
	}
	break;

case SH_HOWLING_OF_CHUL_HO:
{
	int32 range = skill_get_splash(skill_id, skill_lv);

	if (pc_checkskill(sd, SH_COMMUNE_WITH_CHUL_HO) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
	{
		range += 1;
	}

	skill_area_temp[0] = 0;
	skill_area_temp[1] = bl->id;
	skill_area_temp[2] = 0;
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_foreachinrange(skill_area_sub, bl, range, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
}
break;

case SH_HOGOGONG_STRIKE:
	if (pc_checkskill(sd, SH_COMMUNE_WITH_CHUL_HO) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
		status_heal(src, 0, 0, 1, 0);
	skill_area_temp[0] = 0;
	skill_area_temp[1] = bl->id;
	skill_area_temp[2] = 0;
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	break;

case SH_KI_SUL_WATER_SPRAYING:
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
	{
		// TODO: verify on official server, if this should be moved into skill_calc_heal
		int32 heal = 500 * skill_lv + status_get_int(src) * 5;
		heal += pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY) * 100;

		if (pc_checkskill(sd, SH_COMMUNE_WITH_KI_SUL) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
		{
			heal += 250 * skill_lv;
			heal += pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY) * 50;
		}
		heal = heal * (100 + status_get_crt(src)) * status_get_lv(src) / 10000;
		status_heal(bl, heal, 0, 0, 0);
		clif_skill_nodamage(nullptr, *bl, AL_HEAL, heal);
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		int32 range = skill_get_splash(skill_id, skill_lv);
		if (pc_checkskill(sd, SH_COMMUNE_WITH_KI_SUL) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
			range += 2;
		party_foreachsamemap(skill_area_sub, sd, range, src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
	break;

case SH_MARINE_FESTIVAL_OF_KI_SUL:
case SH_SANDY_FESTIVAL_OF_KI_SUL:
	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
	{
		int32 time = skill_get_time(skill_id, skill_lv);
		if (pc_checkskill(sd, SH_COMMUNE_WITH_KI_SUL) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
			time *= 2;
		sc_start(src, bl, type, 100, skill_lv, time);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	}
	else
	{
		int32 range = skill_get_splash(skill_id, skill_lv);
		if (pc_checkskill(sd, SH_COMMUNE_WITH_KI_SUL) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
			range += 2;
		party_foreachsamemap(skill_area_sub, sd, range, src, skill_id, skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
	break;

case SH_KI_SUL_RAMPAGE:
	if (flag & 2)
	{
		if (src == bl)
		{
			break;
		}

		int64 ap = 2;

		if (flag & 4)
		{
			ap += 4;
		}

		status_heal(bl, 0, 0, ap, 0);
	}
	else if (flag & 1)
	{
		int32 range = skill_get_splash(skill_id, skill_lv);

		if (pc_checkskill(sd, SH_COMMUNE_WITH_KI_SUL) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
		{
			range += 2;
			// Set a flag for AP increase
			flag |= 4;
		}

		clif_skill_nodamage(src, *bl, skill_id, 0);
		map_foreachinrange(skill_area_sub, bl, range, BL_CHAR, bl, skill_id, skill_lv, tick, flag | BCT_PARTY | 2, skill_castend_nodamage_id);
	}
	else
	{
		// No party check required
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	break;

case SH_COLORS_OF_HYUN_ROK:
	if (skill_lv == 7)
	{
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_1);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_2);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_3);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_4);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_5);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_6);
		// The skill also ends the buff that increases Catnip Meteor damage
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_BUFF);

		clif_skill_nodamage(src, *src, skill_id, skill_lv);
	}
	else
	{
		// Buff to increase Catnip Meteor damage
		if (pc_checkskill(sd, SH_COMMUNE_WITH_HYUN_ROK) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
			sc_start(src, bl, SC_COLORS_OF_HYUN_ROK_BUFF, 100, 1, skill_get_time(skill_id, skill_lv));

		// Endows elemental property to Catnip Meteor, Hyunrok Breeze and Hyunrok Cannon skills
		switch (skill_lv)
		{
		case 1:
			type = SC_COLORS_OF_HYUN_ROK_1;
			break;
		case 2:
			type = SC_COLORS_OF_HYUN_ROK_2;
			break;
		case 3:
			type = SC_COLORS_OF_HYUN_ROK_3;
			break;
		case 4:
			type = SC_COLORS_OF_HYUN_ROK_4;
			break;
		case 5:
			type = SC_COLORS_OF_HYUN_ROK_5;
			break;
		case 6:
			type = SC_COLORS_OF_HYUN_ROK_6;
			break;
		}
		sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		clif_skill_nodamage(src, *src, skill_id, skill_lv);
	}
	break;

case SH_BLESSING_OF_MYSTICAL_CREATURES:
	status_heal(bl, 0, 0, 200 - status_get_ap(bl), 0);
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *src, skill_id, skill_lv);
	break;

case SS_ANTENPOU:
case SS_KAGENOMAI:
	skill_mirage_cast(*src, nullptr, skill_id, skill_lv, 0, 0, tick, flag | BCT_WOS);
	[[fallthrough]];
case SS_KAGEAKUMU:
case SS_ANKOKURYUUAKUMU:
case SS_HITOUAKUMU:
{
	int32 range = skill_get_splash(skill_id, skill_lv);

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);

	map_foreachinrange(skill_area_sub, bl, range, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
}
break;
case SS_AKUMUKESU:
	if (flag & 1)
	{
		status_change_end(bl, SC_NIGHTMARE);
	}
	else
	{
		int32 range = skill_get_splash(skill_id, skill_lv);

		clif_skill_nodamage(src, *bl, skill_id, skill_lv);

		map_foreachinrange(skill_area_sub, bl, range, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_nodamage_id);
	}
	break;

default:
{
	std::shared_ptr<s_skill_db> skill = skill_db.find(skill_id);
	ShowWarning("skill_castend_nodamage_id: missing code case for skill %s(%d)\n", skill ? skill->name : "UNKNOWN", skill_id);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	map_freeblock_unlock();
	return 1;
}
}

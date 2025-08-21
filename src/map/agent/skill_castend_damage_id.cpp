switch (skill_id)
{
case MER_CRASH:
case MC_MAMMONITE:
case TF_DOUBLE:
case AC_DOUBLE:
case MA_DOUBLE:
case AS_SONICBLOW:
case KN_PIERCE:
case ML_PIERCE:
case KN_SPEARBOOMERANG:
case TF_POISON:
case TF_SPRINKLESAND:
case AC_CHARGEARROW:
case MA_CHARGEARROW:
case RG_INTIMIDATE:
case AM_ACIDTERROR:
case BA_MUSICALSTRIKE:
case DC_THROWARROW:
case BA_DISSONANCE:
case CR_HOLYCROSS:
case NPC_DARKCROSS:
case CR_SHIELDCHARGE:
case CR_SHIELDBOOMERANG:
case NPC_PIERCINGATT:
case NPC_MENTALBREAKER:
case NPC_RANGEATTACK:
case NPC_CRITICALSLASH:
case NPC_COMBOATTACK:
case NPC_GUIDEDATTACK:
case NPC_POISON:
case NPC_RANDOMATTACK:
case NPC_WATERATTACK:
case NPC_GROUNDATTACK:
case NPC_FIREATTACK:
case NPC_WINDATTACK:
case NPC_POISONATTACK:
case NPC_HOLYATTACK:
case NPC_DARKNESSATTACK:
case NPC_TELEKINESISATTACK:
case NPC_UNDEADATTACK:
case NPC_CHANGEUNDEAD:
case NPC_ARMORBRAKE:
case NPC_HELMBRAKE:
case NPC_SHIELDBRAKE:
case NPC_BLINDATTACK:
case NPC_SILENCEATTACK:
case NPC_STUNATTACK:
case NPC_PETRIFYATTACK:
case NPC_CURSEATTACK:
case NPC_SLEEPATTACK:
#ifdef RENEWAL
case CR_ACIDDEMONSTRATION:
#endif
case LK_AURABLADE:
case LK_SPIRALPIERCE:
case ML_SPIRALPIERCE:
case CG_ARROWVULCAN:
case HW_MAGICCRASHER:
case ITM_TOMAHAWK:
case CH_CHAINCRUSH:
case CH_TIGERFIST:
case PA_SHIELDCHAIN: // Shield Chain
case PA_SACRIFICE:
case WS_CARTTERMINATION: // Cart Termination
case AS_VENOMKNIFE:
case HT_PHANTASMIC:
case TK_DOWNKICK:
case TK_COUNTER:
case GS_CHAINACTION:
case GS_TRIPLEACTION:
#ifndef RENEWAL
case GS_MAGICALBULLET:
#endif
case GS_TRACKING:
case GS_PIERCINGSHOT:
case GS_RAPIDSHOWER:
case GS_DUST:
case GS_DISARM: // Added disarm. [Reddozen]
case GS_FULLBUSTER:
case NJ_SYURIKEN:
case NJ_KUNAI:
case ASC_BREAKER:
case HFLI_MOON:	 //[orn]
case HFLI_SBR44: //[orn]
case NPC_BLEEDING:
case NPC_BLEEDING2:
case NPC_CRITICALWOUND:
case NPC_HELLPOWER:
case RK_SONICWAVE:
case AB_DUPLELIGHT_MELEE:
case RA_AIMEDBOLT:
case NC_BOOSTKNUCKLE:
case NC_PILEBUNKER:
case NC_AXEBOOMERANG:
case NC_POWERSWING:
case NPC_MAGMA_ERUPTION:
case NC_MAGMA_ERUPTION:
case GC_WEAPONCRUSH:
case GC_VENOMPRESSURE:
case SC_TRIANGLESHOT:
case SC_FEINTBOMB:
case LG_BANISHINGPOINT:
case LG_SHIELDPRESS:
case LG_RAGEBURST:
case LG_HESPERUSLIT:
case SR_DRAGONCOMBO:
case SR_FALLENEMPIRE:
case SR_CRESCENTELBOW_AUTOSPELL:
case SR_GATEOFHELL:
case SR_GENTLETOUCH_QUIET:
case WM_SEVERE_RAINSTORM_MELEE:
case WM_GREAT_ECHO:
case GN_SLINGITEM_RANGEMELEEATK:
case KO_SETSUDAN:
case RL_MASS_SPIRAL:
case RL_BANISHING_BUSTER:
case RL_SLUGSHOT:
case RL_AM_BLAST:
case BO_ACIDIFIED_ZONE_WATER_ATK:
case BO_ACIDIFIED_ZONE_GROUND_ATK:
case BO_ACIDIFIED_ZONE_WIND_ATK:
case BO_ACIDIFIED_ZONE_FIRE_ATK:
case ABC_CHAIN_REACTION_SHOT_ATK:
case ABR_BATTLE_BUSTER:
case ABR_DUAL_CANNON_FIRE:
case ABR_INFINITY_BUSTER:
case MT_TRIPLE_LASER:
case NW_MISSION_BOMBARD:
case NW_HASTY_FIRE_IN_THE_HOLE:
case NW_BASIC_GRENADE:
case NW_WILD_FIRE:
case SKE_MIDNIGHT_KICK:
case SKE_DAWN_BREAK:
case SKE_RISING_MOON:
case SS_FUUMAKOUCHIKU:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case DK_DRAGONIC_AURA:
case WH_WILD_WALK:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	sc_start(src, src, skill_get_sc(skill_id), 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case DK_DRAGONIC_PIERCE:
case DK_STORMSLASH:
case IG_IMPERIAL_CROSS:
case CD_EFFLIGO:
case ABC_FRENZY_SHOT:
case WH_HAWKRUSH:
case WH_HAWKBOOMERANG:
case TR_ROSEBLOSSOM:
case TR_RHYTHMSHOOTING:
case HN_MEGA_SONIC_BLOW:
case HN_SPIRAL_PIERCE_MAX:
case SH_CHUL_HO_SONIC_CLAW:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case SHC_ETERNAL_SLASH:
	if (sc && sc->getSCE(SC_E_SLASH_COUNT))
		sc_start(src, src, SC_E_SLASH_COUNT, 100, min(5, 1 + sc->getSCE(SC_E_SLASH_COUNT)->val1), skill_get_time(skill_id, skill_lv));
	else
		sc_start(src, src, SC_E_SLASH_COUNT, 100, 1, skill_get_time(skill_id, skill_lv));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case SHC_SHADOW_STAB:
	if (sc && sc->getSCE(SC_CLOAKINGEXCEED))
		flag |= SKILL_ALTDMG_FLAG;

	status_change_end(src, SC_CLOAKING);
	status_change_end(src, SC_CLOAKINGEXCEED);

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case WH_CRESCIVE_BOLT:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	if (sc && sc->getSCE(SC_CRESCIVEBOLT))
		sc_start(src, src, SC_CRESCIVEBOLT, 100, min(3, 1 + sc->getSCE(SC_CRESCIVEBOLT)->val1), skill_get_time(skill_id, skill_lv));
	else
		sc_start(src, src, SC_CRESCIVEBOLT, 100, 1, skill_get_time(skill_id, skill_lv));
	break;

case ABC_UNLUCKY_RUSH:
	// Jump to the target before attacking.
	if (skill_check_unit_movepos(5, src, bl->x, bl->y, 0, 1))
		skill_blown(src, src, 1, (map_calc_dir(bl, src->x, src->y) + 4) % 8, BLOWN_NONE);

	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case MO_TRIPLEATTACK:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag | SD_ANIMATION);
	break;

case LK_HEADCRUSH:
	if (status_get_class_(bl) == CLASS_BOSS)
	{
		if (sd)
			clif_skill_fail(*sd, skill_id);
		break;
	}
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case LK_JOINTBEAT:
	flag = 1 << rnd() % 6;
	if (flag != BREAK_NECK && tsc && tsc->getSCE(SC_JOINTBEAT) && tsc->getSCE(SC_JOINTBEAT)->val2 & BREAK_NECK)
		flag = BREAK_NECK; // Target should always receive double damage if neck is already broken
	if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag))
		status_change_start(src, bl, SC_JOINTBEAT, (50 * (skill_lv + 1) - (270 * tstatus->str) / 100) * 10, skill_lv, flag & BREAK_FLAGS, src->id, 0, skill_get_time2(skill_id, skill_lv), SCSTART_NONE);
	break;

case MO_COMBOFINISH:
	if (!(flag & 1) && sc && sc->getSCE(SC_SPIRIT) && sc->getSCE(SC_SPIRIT)->val2 == SL_MONK)
	{ // Becomes a splash attack when Soul Linked.
		map_foreachinshootrange(skill_area_sub, bl,
								skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL,
								src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1,
								skill_castend_damage_id);
	}
	else
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case TK_STORMKICK: // Taekwon kicks [Dralnu]
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_area_temp[1] = 0;
	map_foreachinshootrange(skill_attack_area, src,
							skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL,
							BF_WEAPON, src, src, skill_id, skill_lv, tick, flag, BCT_ENEMY);
	break;

case KN_CHARGEATK:
{
	bool path = path_search_long(nullptr, src->m, src->x, src->y, bl->x, bl->y, CELL_CHKWALL);
#ifdef RENEWAL
	int32 dist = skill_get_blewcount(skill_id, skill_lv);
#else
	// Charge attack in pre-renewal calculates the distance mathetically
	int32 dist = static_cast<int32>(distance_math_bl(src, bl));
#endif
	uint8 dir = map_calc_dir(bl, src->x, src->y);

	// teleport to target (if not on WoE grounds)
	if (skill_check_unit_movepos(5, src, bl->x + dirx[dir], bl->y + diry[dir], 0, true))
		clif_blown(src);

	// cause damage and knockback if the path to target was a straight one
	if (path)
	{
		if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, dist))
		{
#ifdef RENEWAL
			if (map_getmapdata(src->m)->getMapFlag(MF_PVP))
				dist += 2; // Knockback is 4 on PvP maps
#endif
			skill_blown(src, bl, dist, dir, BLOWN_NONE);
		}
	}
}
break;

case SS_KAGEGISSEN:
	skill_mirage_cast(*src, bl, skill_id, skill_lv, 0, 0, tick, flag);
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	[[fallthrough]];
case NC_FLAMELAUNCHER:
	skill_area_temp[1] = bl->id;
	if (battle_config.skill_eightpath_algorithm)
	{
		// Use official AoE algorithm
		map_foreachindir(skill_attack_area, src->m, src->x, src->y, bl->x, bl->y,
						 skill_get_splash(skill_id, skill_lv), skill_get_maxcount(skill_id, skill_lv), 0, splash_target(src),
						 skill_get_type(skill_id), src, src, skill_id, skill_lv, tick, flag, BCT_ENEMY);
	}
	else
	{
		map_foreachinpath(skill_attack_area, src->m, src->x, src->y, bl->x, bl->y,
						  skill_get_splash(skill_id, skill_lv), skill_get_maxcount(skill_id, skill_lv), splash_target(src),
						  skill_get_type(skill_id), src, src, skill_id, skill_lv, tick, flag, BCT_ENEMY);
	}
	break;

#ifndef RENEWAL
case SN_SHARPSHOOTING:
	flag |= 2; // Flag for specific mob damage formula
	[[fallthrough]];
#endif
case MA_SHARPSHOOTING:
case NJ_KAMAITACHI:
case NPC_DARKPIERCING:
case NPC_ACIDBREATH:
case NPC_DARKNESSBREATH:
case NPC_FIREBREATH:
case NPC_ICEBREATH:
case NPC_ICEBREATH2:
case NPC_THUNDERBREATH:
case AG_STORM_CANNON:
case AG_CRIMSON_ARROW:
	skill_area_temp[1] = bl->id;
	if (skill_id == AG_STORM_CANNON || skill_id == AG_CRIMSON_ARROW)
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	if (battle_config.skill_eightpath_algorithm)
	{
		// Use official AoE algorithm
		if (!(map_foreachindir(skill_attack_area, src->m, src->x, src->y, bl->x, bl->y,
							   skill_get_splash(skill_id, skill_lv), skill_get_maxcount(skill_id, skill_lv), 0, splash_target(src),
							   skill_get_type(skill_id), src, src, skill_id, skill_lv, tick, flag, BCT_ENEMY)))
		{
#ifndef RENEWAL
			if (skill_id == SN_SHARPSHOOTING)
				flag &= ~2; // Only targets in the splash area are affected
#endif

			// These skills hit at least the target if the AoE doesn't hit
			skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
		}
	}
	else
	{
		map_foreachinpath(skill_attack_area, src->m, src->x, src->y, bl->x, bl->y,
						  skill_get_splash(skill_id, skill_lv), skill_get_maxcount(skill_id, skill_lv), splash_target(src),
						  skill_get_type(skill_id), src, src, skill_id, skill_lv, tick, flag, BCT_ENEMY);
	}
	if (skill_id == AG_CRIMSON_ARROW)
		skill_castend_damage_id(src, bl, AG_CRIMSON_ARROW_ATK, skill_lv, tick, flag | SD_LEVEL | SD_ANIMATION);
	break;

case MO_INVESTIGATE:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	status_change_end(src, SC_BLADESTOP);
	break;

case RG_BACKSTAP:
{
#ifdef RENEWAL
	uint8 dir = map_calc_dir(src, bl->x, bl->y);
	int16 x, y;

	if (dir > 0 && dir < 4)
		x = -1;
	else if (dir > 4)
		x = 1;
	else
		x = 0;

	if (dir > 2 && dir < 6)
		y = -1;
	else if (dir == 7 || dir < 2)
		y = 1;
	else
		y = 0;

	if (battle_check_target(src, bl, BCT_ENEMY) > 0 && unit_movepos(src, bl->x + x, bl->y + y, 2, true))
	{ // Display movement + animation.
#else
	if (check_distance_bl(src, bl, 0))
		break;

	uint8 dir = map_calc_dir(src, bl->x, bl->y), t_dir = unit_getdir(bl);

	if (!map_check_dir(dir, t_dir) || bl->type == BL_SKILL)
	{
#endif
		status_change_end(src, SC_HIDING);
		dir = dir < 4 ? dir + 4 : dir - 4; // change direction [Celest]
		unit_setdir(bl, dir);
#ifdef RENEWAL
		clif_blown(src);
#endif
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
}
break;

case MO_FINGEROFFENSIVE:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	if (battle_config.finger_offensive_type && sd)
	{
		for (int32 i = 1; i < sd->spiritball_old; i++)
			skill_addtimerskill(src, tick + i * 200, bl->id, 0, 0, skill_id, skill_lv, BF_WEAPON, flag);
	}
	status_change_end(src, SC_BLADESTOP);
	break;

case MO_CHAINCOMBO:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	status_change_end(src, SC_BLADESTOP);
	break;

#ifndef RENEWAL
case NJ_ISSEN:
#endif
case MO_EXTREMITYFIST:
{
	struct block_list *mbl = bl; // For NJ_ISSEN
	int16 x, y, i = 2;			 // Move 2 cells (From target)
	int16 dir = map_calc_dir(src, bl->x, bl->y);

#ifdef RENEWAL
	if (skill_id == MO_EXTREMITYFIST && sd && sd->spiritball_old > 5)
		flag |= 1; // Give +100% damage increase
#endif
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	if (skill_id == MO_EXTREMITYFIST)
	{
		status_set_sp(src, 0, 0);
		sc_start(src, src, SC_EXTREMITYFIST, 100, skill_lv, skill_get_time(skill_id, skill_lv));
		status_change_end(src, SC_EXPLOSIONSPIRITS);
		status_change_end(src, SC_BLADESTOP);
	}
	else
	{
		status_set_hp(src, 1, 0);
		status_change_end(src, SC_NEN);
		status_change_end(src, SC_HIDING);
	}
	if (skill_id == MO_EXTREMITYFIST)
	{
		mbl = src; // For MO_EXTREMITYFIST
		i = 3;	   // Move 3 cells (From caster)
	}
	if (dir > 0 && dir < 4)
		x = -i;
	else if (dir > 4)
		x = i;
	else
		x = 0;
	if (dir > 2 && dir < 6)
		y = -i;
	else if (dir == 7 || dir < 2)
		y = i;
	else
		y = 0;
	// Ashura Strike still has slide effect in GVG
	if ((mbl == src || (!map_flag_gvg2(src->m) && !map_getmapflag(src->m, MF_BATTLEGROUND))) &&
		unit_movepos(src, mbl->x + x, mbl->y + y, 1, 1))
	{
		clif_blown(src);
		clif_spiritball(src);
	}
}
break;

case HT_POWER:
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_PLAYER_DORAM || tstatus->race == RC_INSECT)
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case SU_PICKYPECK:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	[[fallthrough]];
case SU_BITE:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case SU_SVG_SPIRIT:
	skill_area_temp[1] = bl->id;
	map_foreachinpath(skill_attack_area, src->m, src->x, src->y, bl->x, bl->y,
					  skill_get_splash(skill_id, skill_lv), skill_get_maxcount(skill_id, skill_lv), splash_target(src),
					  skill_get_type(skill_id), src, src, skill_id, skill_lv, tick, flag, BCT_ENEMY);
	break;

// Splash attack skills.
case AS_GRIMTOOTH:
case MC_CARTREVOLUTION:
case NPC_SPLASHATTACK:
	flag |= SD_PREAMBLE; // a fake packet will be sent for the first target to be hit
	[[fallthrough]];
case AS_SPLASHER:
case HT_BLITZBEAT:
case AC_SHOWER:
case MA_SHOWER:
case MG_NAPALMBEAT:
case MG_FIREBALL:
case RG_RAID:
#ifdef RENEWAL
case SN_SHARPSHOOTING:
#endif
case HW_NAPALMVULCAN:
case NJ_HUUMA:
case ASC_METEORASSAULT:
case GS_SPREADATTACK:
case NPC_PULSESTRIKE:
case NPC_PULSESTRIKE2:
case NPC_HELLJUDGEMENT:
case NPC_HELLJUDGEMENT2:
case NPC_VAMPIRE_GIFT:
case NPC_MAXPAIN_ATK:
case NPC_JACKFROST:
case NPC_REVERBERATION_ATK:
case NPC_ARROWSTORM:
case NPC_KILLING_AURA:
case NPC_IGNITIONBREAK:
case RK_IGNITIONBREAK:
case RK_HUNDREDSPEAR:
case AB_JUDEX:
case AB_ADORAMUS:
case WL_SOULEXPANSION:
case WL_CRIMSONROCK:
case WL_JACKFROST:
case RA_ARROWSTORM:
case RA_WUGDASH:
case NC_VULCANARM:
case NC_COLDSLOWER:
case NC_SELFDESTRUCTION:
case NC_AXETORNADO:
case GC_ROLLINGCUTTER:
case GC_COUNTERSLASH:
case LG_CANNONSPEAR:
case LG_OVERBRAND:
case LG_MOONSLASHER:
case LG_RAYOFGENESIS:
case NPC_RAYOFGENESIS:
case LG_EARTHDRIVE:
case SR_RAMPAGEBLASTER:
case SR_SKYNETBLOW:
case SR_WINDMILL:
case SR_RIDEINLIGHTNING:
case SO_VARETYR_SPEAR:
case SO_POISON_BUSTER:
case GN_CART_TORNADO:
case GN_CARTCANNON:
case GN_SPORE_EXPLOSION:
case GN_DEMONIC_FIRE:
case GN_FIRE_EXPANSION_ACID:
case GN_HELLS_PLANT_ATK:
case KO_HAPPOKUNAI:
case KO_HUUMARANKA:
case KO_MUCHANAGE:
case KO_BAKURETSU:
case GN_ILLUSIONDOPING:
case RL_FIREDANCE:
case RL_S_STORM:
case RL_R_TRIP:
case MH_XENO_SLASHER:
case MH_HEILIGE_PFERD:
case MH_THE_ONE_FIGHTER_RISES:
case NC_ARMSCANNON:
case SU_SCRATCH:
case SU_LUNATICCARROTBEAT:
case SU_LUNATICCARROTBEAT2:
case SJ_FULLMOONKICK:
case SJ_NEWMOONKICK:
case SJ_SOLARBURST:
case SJ_PROMINENCEKICK:
case SJ_STAREMPEROR:
case SJ_FALLINGSTAR_ATK2:
case SP_CURSEEXPLOSION:
case SP_SHA:
case SP_SWHOO:
case DK_SERVANTWEAPON_ATK:
case DK_SERVANT_W_PHANTOM:
case DK_SERVANT_W_DEMOL:
case DK_MADNESS_CRUSHER:
case DK_HACKANDSLASHER:
case DK_DRAGONIC_BREATH:
case AG_CRIMSON_ARROW_ATK:
case AG_DESTRUCTIVE_HURRICANE:
case AG_SOUL_VC_STRIKE:
case AG_CRYSTAL_IMPACT:
case AG_CRYSTAL_IMPACT_ATK:
case AG_ROCK_DOWN:
case AG_FROZEN_SLASH:
case IQ_OLEUM_SANCTUM:
case IQ_MASSIVE_F_BLASTER:
case IQ_EXPOSION_BLASTER:
case IQ_FIRST_BRAND:
case IQ_SECOND_FLAME:
case IQ_SECOND_FAITH:
case IQ_SECOND_JUDGEMENT:
case IQ_THIRD_PUNISH:
case IQ_THIRD_FLAME_BOMB:
case IQ_THIRD_CONSECRATION:
case IG_OVERSLASH:
case IG_RADIANT_SPEAR:
case IG_IMPERIAL_PRESSURE:
case CD_ARBITRIUM_ATK:
case CD_PETITIO:
case CD_FRAMEN:
case SHC_DANCING_KNIFE:
case SHC_SAVAGE_IMPACT:
case SHC_IMPACT_CRATER:
case SHC_FATAL_SHADOW_CROW:
case SHC_CROSS_SLASH:
case MT_AXE_STOMP:
case MT_MIGHTY_SMASH:
case MT_RUSH_QUAKE:
case MT_A_MACHINE:
case MT_SPARK_BLASTER:
case MT_RUSH_STRIKE:
case MT_POWERFUL_SWING:
case MT_ENERGY_CANNONADE:
case ABC_ABYSS_DAGGER:
case ABC_CHAIN_REACTION_SHOT:
case ABC_DEFT_STAB:
case WH_GALESTORM:
case BO_ACIDIFIED_ZONE_WATER:
case BO_ACIDIFIED_ZONE_GROUND:
case BO_ACIDIFIED_ZONE_WIND:
case BO_ACIDIFIED_ZONE_FIRE:
case TR_ROSEBLOSSOM_ATK:
case ABC_FROM_THE_ABYSS_ATK:
case EM_ELEMENTAL_BUSTER_FIRE:
case EM_ELEMENTAL_BUSTER_WATER:
case EM_ELEMENTAL_BUSTER_WIND:
case EM_ELEMENTAL_BUSTER_GROUND:
case EM_ELEMENTAL_BUSTER_POISON:
case EM_EL_FLAMEROCK:
case EM_EL_AGE_OF_ICE:
case EM_EL_STORM_WIND:
case EM_EL_AVALANCHE:
case EM_EL_DEADLY_POISON:
case BO_EXPLOSIVE_POWDER:
case BO_MAYHEMIC_THORNS:
case BO_MYSTERY_POWDER:
case BO_DUST_EXPLOSION:
case NPC_WIDECRITICALWOUND:
case IG_SHIELD_SHOOTING:
case TR_METALIC_FURY:
case IG_GRAND_JUDGEMENT:
case HN_JUPITEL_THUNDER_STORM:
case SH_CHUL_HO_BATTERING:
case SH_HYUN_ROK_SPIRIT_POWER:
case SOA_EXORCISM_OF_MALICIOUS_SOUL:
case SOA_TALISMAN_OF_WHITE_TIGER:
case SOA_TALISMAN_OF_RED_PHOENIX:
case SOA_TALISMAN_OF_FOUR_BEARING_GOD:
case SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS:
case SKE_SUNSET_BLAST:
case SKE_NOON_BLAST:
case SS_KINRYUUHOU:
	if (flag & 1)
	{ // Recursive invocation
		int32 sflag = skill_area_temp[0] & 0xFFF;
		int32 heal = 0;
		std::bitset<INF2_MAX> inf2 = skill_db.find(skill_id)->inf2;

		if (tsc && tsc->getSCE(SC_HOVERING) && inf2[INF2_IGNOREHOVERING])
			break; // Under Hovering characters are immune to select trap and ground target skills.

		if (skill_id == AB_ADORAMUS && map_getcell(bl->m, bl->x, bl->y, CELL_CHKLANDPROTECTOR))
			break; // No damage should happen if the target is on Land Protector

		// Servant Weapon - Demol only hits if the target is marked with a sign by the attacking caster.
		if (skill_id == DK_SERVANT_W_DEMOL && !(tsc && tsc->getSCE(SC_SERVANT_SIGN) && tsc->getSCE(SC_SERVANT_SIGN)->val1 == src->id))
			break;

		switch (skill_id)
		{
		case MG_FIREBALL:
			// For players, the distance between original target and splash target determines the damage
			if (sd != nullptr)
			{
				if (block_list *orig_bl = map_id2bl(skill_area_temp[1]); orig_bl != nullptr)
					sflag |= distance_bl(orig_bl, bl);
			}
			break;
		case ABC_DEFT_STAB:
			// Deft Stab - Make sure the flag of 2 is passed on when the skill is double casted.
			if (flag & 2)
				sflag |= 2;
			break;
		}

		if (flag & SD_LEVEL)
			sflag |= SD_LEVEL; // -1 will be used in packets instead of the skill level
		if (skill_area_temp[1] != bl->id && !inf2[INF2_ISNPC])
			sflag |= SD_ANIMATION; // original target gets no animation (as well as all NPC skills)

		// If a enemy player is standing next to a mob when splash Es- skill is casted, the player won't get hurt.
		if ((skill_id == SP_SHA || skill_id == SP_SWHOO) && !battle_config.allow_es_magic_pc && bl->type != BL_MOB)
			break;

		heal = (int32)skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, sflag);

		switch (skill_id)
		{
		case NPC_VAMPIRE_GIFT:
			if (heal > 0)
			{
				clif_skill_nodamage(nullptr, *src, AL_HEAL, heal);
				status_heal(src, heal, 0, 0);
			}
			break;
#ifdef RENEWAL
		case SN_SHARPSHOOTING:
			status_change_end(src, SC_CAMOUFLAGE);
			break;
#endif
		case SJ_PROMINENCEKICK: // Trigger the 2nd hit. (100% fire damage.)
			skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, sflag | 8 | SD_ANIMATION);
			break;
		}
	}
	else
	{
		int32 starget = BL_CHAR | BL_SKILL, splash_size = skill_get_splash(skill_id, skill_lv);

		skill_area_temp[0] = 0;
		skill_area_temp[1] = bl->id;
		skill_area_temp[2] = 0;

		if (sd && (skill_id == SP_SHA || skill_id == SP_SWHOO) && !battle_config.allow_es_magic_pc && bl->type != BL_MOB)
		{
			status_change_start(src, bl, SC_STUN, 10000, skill_lv, 0, 0, 0, 500, 10);
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
			break;
		}

		if (skill_id == SP_SWHOO)
			status_change_end(src, SC_USE_SKILL_SP_SPA);

		switch (skill_id)
		{
		case GN_CARTCANNON:
		case SU_SCRATCH:
		case IG_IMPERIAL_PRESSURE:
		case BO_MAYHEMIC_THORNS:
		case DK_DRAGONIC_BREATH:
		case DK_HACKANDSLASHER:
		case MT_SPARK_BLASTER:
		case HN_JUPITEL_THUNDER_STORM:
		case SH_CHUL_HO_BATTERING:
		case SH_HYUN_ROK_SPIRIT_POWER:
		case SOA_TALISMAN_OF_FOUR_BEARING_GOD:
		case SKE_SUNSET_BLAST:
		case SKE_NOON_BLAST:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			break;
#ifdef RENEWAL
		case NJ_HUUMA:
#endif
		case LG_MOONSLASHER:
		case MH_XENO_SLASHER:
			clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
			break;
		case NPC_REVERBERATION_ATK:
		case NC_ARMSCANNON:
			skill_area_temp[1] = 0;
			starget = splash_target(src);
			break;
		case WL_CRIMSONROCK:
			skill_area_temp[4] = bl->x;
			skill_area_temp[5] = bl->y;
			break;
		case SU_LUNATICCARROTBEAT:
			if (sd && pc_search_inventory(sd, skill_db.find(SU_LUNATICCARROTBEAT)->require.itemid[0]) >= 0)
				skill_id = SU_LUNATICCARROTBEAT2;
			break;
		case DK_SERVANT_W_PHANTOM:
			// Jump to the target before attacking.
			if (skill_check_unit_movepos(5, src, bl->x, bl->y, 0, 1))
				skill_blown(src, src, 1, (map_calc_dir(bl, src->x, src->y) + 4) % 8, BLOWN_NONE);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv); // Trigger animation on servants.
			break;
		case SHC_SAVAGE_IMPACT:
		{
			if (sc && sc->getSCE(SC_CLOAKINGEXCEED))
			{
				skill_area_temp[0] = 2;
				status_change_end(src, SC_CLOAKINGEXCEED);
			}

			uint8 dir = DIR_NORTHEAST; // up-right when src is on the same cell of target

			if (bl->x != src->x || bl->y != src->y)
				dir = map_calc_dir(bl, src->x, src->y); // dir based on target as we move player based on target location

			// Move the player 1 cell near the target, between the target and the player
			if (skill_check_unit_movepos(5, src, bl->x + dirx[dir], bl->y + diry[dir], 0, 1))
				clif_blown(src);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			break;
		}
		case SHC_FATAL_SHADOW_CROW:
		{
			uint8 dir = DIR_NORTHEAST;

			if (bl->x != src->x || bl->y != src->y)
				dir = map_calc_dir(bl, src->x, src->y); // dir based on target as we move player based on target location

			// Move the player 1 cell near the target, between the target and the player
			if (skill_check_unit_movepos(5, src, bl->x + dirx[dir], bl->y + diry[dir], 0, 1))
				clif_blown(src);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv); // Trigger animation
			break;
		}
		case AG_CRYSTAL_IMPACT_ATK:
			if (sc && sc->getSCE(SC_CLIMAX) && sc->getSCE(SC_CLIMAX)->val1 == 5)
				splash_size = 2; // Gives the aftershock hit a 5x5 splash AoE.
			break;
		case AG_ROCK_DOWN:
		case IQ_FIRST_BRAND:
		case IQ_SECOND_FLAME:
		case IQ_SECOND_FAITH:
		case IQ_SECOND_JUDGEMENT:
		case IG_RADIANT_SPEAR:
		case CD_PETITIO:
		case CD_FRAMEN:
		case MT_POWERFUL_SWING:
		case MT_ENERGY_CANNONADE:
		case BO_DUST_EXPLOSION:
		case ABC_DEFT_STAB:
		case EM_EL_FLAMEROCK:
		case EM_EL_AGE_OF_ICE:
		case EM_EL_STORM_WIND:
		case EM_EL_AVALANCHE:
		case EM_EL_DEADLY_POISON:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			break;
		case ABC_CHAIN_REACTION_SHOT:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			map_foreachinrange(skill_area_sub, bl, skill_get_splash(ABC_CHAIN_REACTION_SHOT_ATK, skill_lv), BL_CHAR | BL_SKILL, src, ABC_CHAIN_REACTION_SHOT_ATK, skill_lv, tick + (200 + status_get_amotion(src)), flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
			break;
		case IQ_THIRD_PUNISH:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			if (sd)
			{
				uint8 limit = 5;

				if (sc && sc->getSCE(SC_RAISINGDRAGON))
					limit += sc->getSCE(SC_RAISINGDRAGON)->val1;
				for (uint8 i = 0; i < limit; i++)
					pc_addspiritball(sd, skill_get_time(skill_id, skill_lv), limit);
			}
			break;
		case IQ_THIRD_FLAME_BOMB:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			if (sd && sd->spiritball / 5 > 1)
				skill_area_temp[0] = sd->spiritball / 5 - 1;
			break;
		case IQ_THIRD_CONSECRATION:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			status_heal(src, status_get_max_hp(src) * skill_lv / 100, status_get_max_sp(src) * skill_lv / 100, 0);
			break;
		case IG_OVERSLASH:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			skill_area_temp[0] = map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
			break;
		case WH_GALESTORM: // Give AP if 3 or more targets are hit.
			if (sd && map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, BCT_ENEMY, skill_area_sub_count) >= 3)
				status_heal(src, 0, 0, 10, 0);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			break;
		case BO_ACIDIFIED_ZONE_WATER:
		case BO_ACIDIFIED_ZONE_GROUND:
		case BO_ACIDIFIED_ZONE_WIND:
		case BO_ACIDIFIED_ZONE_FIRE:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			if (bl->type == BL_PC) // Place single cell AoE if hitting a player.
				skill_castend_pos2(src, bl->x, bl->y, skill_id, skill_lv, tick, 0);
			break;
		case MT_RUSH_STRIKE:
		case MT_RUSH_QUAKE:
			// Jump to the target before attacking.
			if (skill_check_unit_movepos(5, src, bl->x, bl->y, 0, 1))
			{
				skill_blown(src, src, 1, direction_opposite(static_cast<enum directions>(map_calc_dir(bl, src->x, src->y))), BLOWN_NONE);
			}
			clif_skill_nodamage(src, *bl, skill_id, skill_lv); // Trigger animation
			clif_blown(src);
			if (skill_id == MT_RUSH_QUAKE)
			{
				// TODO: does this buff start before or after dealing damage? [Muh]
				sc_start(src, src, SC_RUSH_QUAKE2, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
			}
			break;
		case BO_MYSTERY_POWDER:
		case IG_SHIELD_SHOOTING:
		case IG_GRAND_JUDGEMENT:
		case SHC_CROSS_SLASH:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			sc_start(src, src, skill_get_sc(skill_id), 100, skill_lv, skill_get_time(skill_id, skill_lv));
			break;
		case SOA_TALISMAN_OF_RED_PHOENIX:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			skill_area_temp[0] = map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
			if (sc != nullptr && sc->getSCE(SC_T_SECOND_GOD) != nullptr)
			{
				sc_start(src, src, skill_get_sc(skill_id), 100, skill_lv, skill_get_time(skill_id, skill_lv));
			}
			break;
		case SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS:
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			skill_area_temp[0] = map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
			sc_start(src, src, skill_get_sc(skill_id), 100, skill_lv, skill_get_time(skill_id, skill_lv));
			break;
		case SS_KINRYUUHOU:
			skill_mirage_cast(*src, nullptr, SS_ANTENPOU, skill_lv, 0, 0, tick, flag | BCT_WOS);
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);
			break;
		}

		// if skill damage should be split among targets, count them
		// SD_LEVEL -> Forced splash damage for Auto Blitz-Beat -> count targets
		// special case: Venom Splasher uses a different range for searching than for splashing
		if (flag & SD_LEVEL || skill_get_nk(skill_id, NK_SPLASHSPLIT))
		{
			skill_area_temp[0] = map_foreachinallrange(skill_area_sub, bl, (skill_id == AS_SPLASHER) ? 1 : splash_size, BL_CHAR, src, skill_id, skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
			// If there are no characters in the area, then it always counts as if there was one target
			// This happens when targetting skill units such as icewall
			skill_area_temp[0] = std::max(1, skill_area_temp[0]);
		}

		// recursive invocation of skill_castend_damage_id() with flag|1
		map_foreachinrange(skill_area_sub, bl, splash_size, starget, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);

		if (skill_id == RA_ARROWSTORM)
			status_change_end(src, SC_CAMOUFLAGE);
		if (skill_id == AS_SPLASHER)
		{
			map_freeblock_unlock(); // Don't consume a second gemstone.
			return 0;
		}
	}
	break;
case NW_THE_VIGILANTE_AT_NIGHT:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case NW_SPIRAL_SHOOTING:
	if (flag & 1)
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	}
	else
	{
		int32 splash = skill_get_splash(skill_id, skill_lv);

		if (sd && sd->weapontype1 == W_GRENADE)
			splash += 2;
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinrange(skill_area_sub, bl, splash, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
			status_change_end(src, SC_INTENSIVE_AIM_COUNT);
	}
	break;

case SH_HOWLING_OF_CHUL_HO:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case SH_HOGOGONG_STRIKE:
	if (flag & 1 && tsc != nullptr && tsc->getSCE(SC_HOGOGONG) != nullptr)
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	}
	break;

case SKE_RISING_SUN:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);

	if (sc == nullptr || (sc->getSCE(SC_RISING_SUN) == nullptr && sc->getSCE(SC_NOON_SUN) == nullptr && sc->getSCE(SC_SUNSET_SUN) == nullptr))
	{
		sc_start(src, src, SC_RISING_SUN, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else if (sc->getSCE(SC_NOON_SUN) == nullptr && sc->getSCE(SC_SUNSET_SUN) == nullptr)
	{
		sc_start(src, src, SC_NOON_SUN, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else if (sc->getSCE(SC_SUNSET_SUN) == nullptr)
	{
		sc_start(src, src, SC_SUNSET_SUN, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	}

	break;

case SKE_TWINKLING_GALAXY:
case SKE_STAR_CANNON:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case SKE_STAR_BURST:
	if (flag & 1)
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	}
	else
	{
		unit_data *ud = unit_bl2ud(src);

		if (ud != nullptr)
		{
			for (const std::shared_ptr<s_skill_unit_group> &sug : ud->skillunits)
			{
				if (sug->skill_id != SKE_TWINKLING_GALAXY)
				{
					continue;
				}

				skill_unit *su = sug->unit;

				// Check if it is too far away
				if (distance_xy(bl->x, bl->y, su->x, su->y) > skill_get_unit_range(sug->skill_id, sug->skill_lv))
				{
					continue;
				}

				std::shared_ptr<s_skill_unit_group> sg = su->group;

				for (int32 i = 0; i < MAX_SKILLTIMERSKILL; i++)
				{
					if (ud->skilltimerskill[i] == nullptr)
					{
						continue;
					}

					if (ud->skilltimerskill[i]->skill_id != sug->skill_id)
					{
						continue;
					}

					delete_timer(ud->skilltimerskill[i]->timer, skill_timerskill);
					ers_free(skill_timer_ers, ud->skilltimerskill[i]);
					ud->skilltimerskill[i] = nullptr;
				}

				skill_delunitgroup(sg);
				sc_start2(src, bl, skill_get_sc(skill_id), 100, skill_lv, src->id, skill_get_time2(skill_id, skill_lv));

				return skill_castend_pos2(src, bl->x, bl->y, skill_id, skill_lv, tick, 0);
			}
		}

		if (sd != nullptr)
		{
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL_LEVEL);
		}

		return 1;
	}
	break;

case SKE_ALL_IN_THE_SKY:
	if (bl->type == BL_PC)
		status_zap(bl, 0, 0, status_get_ap(bl));
	if (unit_movepos(src, bl->x, bl->y, 2, true))
	{
		clif_snap(src, src->x, src->y);
	}
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case SS_KAGEGARI:
case SS_TOKEDASU:
case SS_KAGEAKUMU:
case SS_HITOUAKUMU:
case SS_KAGENOMAI:
case SS_ANTENPOU:
case SS_REIKETSUHOU:
case SS_KUNAIWAIKYOKU:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case SS_ANKOKURYUUAKUMU:
	if (flag & 1)
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);

		if (tsc != nullptr && tsc->getSCE(SC_NIGHTMARE) != nullptr)
		{
			skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag | SKILL_ALTDMG_FLAG);
		}
	}
	break;
case SS_SHIMIRU:
{
	struct unit_data *ud = unit_bl2ud(src);

	if (!check_distance_bl(src, bl, 0))
	{
		uint8 dir = map_calc_dir(src, bl->x, bl->y);
		int16 x, y;

		if (dir > DIR_NORTH && dir < DIR_SOUTH)
			x = -1;
		else if (dir > DIR_SOUTH)
			x = 1;
		else
			x = 0;

		if (dir > DIR_WEST && dir < DIR_EAST)
			y = -1;
		else if (dir == DIR_NORTHEAST || dir < DIR_WEST)
			y = 1;
		else
			y = 0;

		if (battle_check_target(src, bl, BCT_ENEMY) > 0 && unit_movepos(src, bl->x + x, bl->y + y, 2, true))
		{									   // Display movement + animation.
			dir = dir < 4 ? dir + 4 : dir - 4; // change direction [Celest]
			unit_setdir(bl, dir);
			clif_blown(src);
		}
		else if (sd)
		{
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL_TARGET_SHADOW_SPACE);
			break;
		}
	}

	if (ud == nullptr)
		break;

	for (const std::shared_ptr<s_skill_unit_group> &sug : ud->skillunits)
	{
		skill_unit *su = sug->unit;
		std::shared_ptr<s_skill_unit_group> sg = su->group;
		int16 dx = src->x - su->x;
		int16 dy = src->y - su->y;

		for (size_t count = 0; count < 1000; count++)
		{
			if (map_foreachincell(skill_shimiru_check_cell, src->m, su->x + dx, su->y + dy, BL_CHAR | BL_SKILL) == 0)
				break;
			dx += rnd() % 3 - 1;
			dy += rnd() % 3 - 1;
		}

		if (sug->skill_id == SS_SHINKIROU)
			skill_unit_move_unit_group(sg, src->m, dx, dy);
	}

	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);

	sc_start(src, src, skill_get_sc(skill_id), 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	break;
}

// Place units around target
case NJ_BAKUENRYU:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_unitsetting(src, skill_id, skill_lv, bl->x, bl->y, 0);
	break;

case SM_MAGNUM:
case MS_MAGNUM:
	if (flag & 1)
	{
		// For players, damage depends on distance, so add it to flag if it is > 1
		// Cannot hit hidden targets
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag | SD_ANIMATION | (sd ? distance_bl(src, bl) : 0));
	}
	break;

#ifdef RENEWAL
case KN_BRANDISHSPEAR:
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;
#else
case KN_BRANDISHSPEAR:
#endif
case ML_BRANDISH:
	// Coded apart for it needs the flag passed to the damage calculation.
	if (skill_area_temp[1] != bl->id)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag | SD_ANIMATION);
	else
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;

#ifdef RENEWAL
case KN_BOWLINGBASH:
	if (flag & 1)
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, (skill_area_temp[0]) > 0 ? SD_ANIMATION | skill_area_temp[0] : skill_area_temp[0]);
	}
	else
	{
		skill_area_temp[0] = map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
	break;
#else
case KN_BOWLINGBASH:
#endif
case MS_BOWLINGBASH:
{
	int32 min_x, max_x, min_y, max_y, i, c, dir, tx, ty;
	// Chain effect and check range gets reduction by recursive depth, as this can reach 0, we don't use blowcount
	c = (skill_lv - (flag & 0xFFF) + 1) / 2;
	// Determine the Bowling Bash area depending on configuration
	if (battle_config.bowling_bash_area == 0)
	{
		// Gutter line system
		min_x = ((src->x) - c) - ((src->x) - c) % 40;
		if (min_x < 0)
			min_x = 0;
		max_x = min_x + 39;
		min_y = ((src->y) - c) - ((src->y) - c) % 40;
		if (min_y < 0)
			min_y = 0;
		max_y = min_y + 39;
	}
	else if (battle_config.bowling_bash_area == 1)
	{
		// Gutter line system without demi gutter bug
		min_x = src->x - (src->x) % 40;
		max_x = min_x + 39;
		min_y = src->y - (src->y) % 40;
		max_y = min_y + 39;
	}
	else
	{
		// Area around caster
		min_x = src->x - battle_config.bowling_bash_area;
		max_x = src->x + battle_config.bowling_bash_area;
		min_y = src->y - battle_config.bowling_bash_area;
		max_y = src->y + battle_config.bowling_bash_area;
	}
	// Initialization, break checks, direction
	if ((flag & 0xFFF) > 0)
	{
		// Ignore monsters outside area
		if (bl->x < min_x || bl->x > max_x || bl->y < min_y || bl->y > max_y)
			break;
		// Ignore monsters already in list
		if (idb_exists(bowling_db, bl->id))
			break;
		// Random direction
		dir = rnd() % 8;
	}
	else
	{
		// Create an empty list of already hit targets
		db_clear(bowling_db);
		// Direction is walkpath
		dir = (unit_getdir(src) + 4) % 8;
	}
	// Add current target to the list of already hit targets
	idb_put(bowling_db, bl->id, bl);
	// Keep moving target in direction square by square
	tx = bl->x;
	ty = bl->y;
	for (i = 0; i < c; i++)
	{
		// Target coordinates (get changed even if knockback fails)
		tx -= dirx[dir];
		ty -= diry[dir];
		// If target cell is a wall then break
		if (map_getcell(bl->m, tx, ty, CELL_CHKWALL))
			break;
		skill_blown(src, bl, 1, dir, BLOWN_NONE);

		int32 count;

		// Splash around target cell, but only cells inside area; we first have to check the area is not negative
		if ((max(min_x, tx - 1) <= min(max_x, tx + 1)) &&
			(max(min_y, ty - 1) <= min(max_y, ty + 1)) &&
			(count = map_foreachinallarea(skill_area_sub, bl->m, max(min_x, tx - 1), max(min_y, ty - 1), min(max_x, tx + 1), min(max_y, ty + 1), splash_target(src), src, skill_id, skill_lv, tick, flag | BCT_ENEMY, skill_area_sub_count)))
		{
			// Recursive call
			map_foreachinallarea(skill_area_sub, bl->m, max(min_x, tx - 1), max(min_y, ty - 1), min(max_x, tx + 1), min(max_y, ty + 1), splash_target(src), src, skill_id, skill_lv, tick, (flag | BCT_ENEMY) + 1, skill_castend_damage_id);
			// Self-collision
			if (bl->x >= min_x && bl->x <= max_x && bl->y >= min_y && bl->y <= max_y)
				skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, (flag & 0xFFF) > 0 ? SD_ANIMATION | count : count);
			break;
		}
	}
	// Original hit or chain hit depending on flag
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, (flag & 0xFFF) > 0 ? SD_ANIMATION : 0);
}
break;

case KN_SPEARSTAB:
	if (flag & 1)
	{
		if (bl->id == skill_area_temp[1])
			break;
		if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, SD_ANIMATION))
			skill_blown(src, bl, skill_area_temp[2], -1, BLOWN_NONE);
	}
	else
	{
		int32 x = bl->x, y = bl->y, i, dir;
		dir = map_calc_dir(bl, src->x, src->y);
		skill_area_temp[1] = bl->id;
		skill_area_temp[2] = skill_get_blewcount(skill_id, skill_lv);
		// all the enemies between the caster and the target are hit, as well as the target
		if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, 0))
			skill_blown(src, bl, skill_area_temp[2], -1, BLOWN_NONE);
		for (i = 0; i < 4; i++)
		{
			map_foreachincell(skill_area_sub, bl->m, x, y, BL_CHAR,
							  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
			x += dirx[dir];
			y += diry[dir];
		}
	}
	break;

case TK_TURNKICK:
case MO_BALKYOUNG: // Active part of the attack. Skill-attack [Skotlex]
{
	skill_area_temp[1] = bl->id; // NOTE: This is used in skill_castend_nodamage_id to avoid affecting the target.
	if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag))
		map_foreachinallrange(skill_area_sub, bl,
							  skill_get_splash(skill_id, skill_lv), (skill_id == TK_TURNKICK) ? BL_MOB : BL_CHAR,
							  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1,
							  skill_castend_nodamage_id);
}
break;
case CH_PALMSTRIKE:																		   //	Palm Strike takes effect 1sec after casting. [Skotlex]
																						   //	clif_skill_nodamage(src,*bl,skill_id,skill_lv,false); //Can't make this one display the correct attack animation delay :/
	clif_damage(*src, *bl, tick, status_get_amotion(src), 0, -1, 1, DMG_ENDURE, 0, false); // Display an absorbed damage attack.
	skill_addtimerskill(src, tick + (1000 + status_get_amotion(src)), bl->id, 0, 0, skill_id, skill_lv, BF_WEAPON, flag);
	break;

case PR_TURNUNDEAD:
case ALL_RESURRECTION:
	if (!battle_check_undead(tstatus->race, tstatus->def_ele))
		break;
	skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case AL_HOLYLIGHT:
	status_change_end(bl, SC_P_ALTER);
	[[fallthrough]];
case MG_SOULSTRIKE:
case NPC_DARKSTRIKE:
case MG_COLDBOLT:
case MG_FIREBOLT:
case MG_LIGHTNINGBOLT:
case WZ_EARTHSPIKE:
case AL_HEAL:
case NPC_DARKTHUNDER:
case NPC_FIRESTORM:
case PR_ASPERSIO:
case MG_FROSTDIVER:
case WZ_SIGHTBLASTER:
case WZ_SIGHTRASHER:
#ifdef RENEWAL
case PA_PRESSURE:
#endif
case NJ_KOUENKA:
case NJ_HYOUSENSOU:
case NJ_HUUJIN:
case AB_HIGHNESSHEAL:
case AB_DUPLELIGHT_MAGIC:
case WM_METALICSOUND:
case KO_KAIHOU:
case MH_ERASER_CUTTER:
case AG_ASTRAL_STRIKE:
case AG_ASTRAL_STRIKE_ATK:
case AG_DESTRUCTIVE_HURRICANE_CLIMAX:
case CD_ARBITRIUM:
case HN_METEOR_STORM_BUSTER:
	skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case IG_JUDGEMENT_CROSS:
case TR_SOUNDBLEND:
case SH_HYUN_ROK_CANNON:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case AG_DEADLY_PROJECTION:
	sc_start(src, bl, SC_DEADLY_DEFEASANCE, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case NPC_MAGICALATTACK:
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	sc_start(src, src, SC_MAGICALATTACK, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case HVAN_CAPRICE:
{
	static const std::array<e_skill, 4> subskills = {MG_COLDBOLT, MG_FIREBOLT, MG_LIGHTNINGBOLT, WZ_EARTHSPIKE};
	e_skill subskill_id = subskills.at(rnd() % subskills.size());
	skill_attack(skill_get_type(subskill_id), src, src, bl, subskill_id, skill_lv, tick, flag);
}
break;

case WZ_WATERBALL:
	// Deploy waterball cells, these are used and turned into waterballs via the timerskill
	skill_unitsetting(src, skill_id, skill_lv, src->x, src->y, 0);
	skill_addtimerskill(src, tick, bl->id, src->x, src->y, skill_id, skill_lv, 0, flag);
	break;
case WZ_JUPITEL:
	// Jupitel Thunder is delayed by 150ms, you can cast another spell before the knockback
	skill_addtimerskill(src, tick + TIMERSKILL_INTERVAL, bl->id, 0, 0, skill_id, skill_lv, 1, flag);
	break;

case PR_BENEDICTIO:
	// Should attack undead and demons. [Skotlex]
	if (battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON)
		skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case SJ_NOVAEXPLOSING:
	skill_attack(BF_MISC, src, src, bl, skill_id, skill_lv, tick, flag);

	// We can end Dimension here since the cooldown code is processed before this point.
	if (sc && sc->getSCE(SC_DIMENSION))
		status_change_end(src, SC_DIMENSION);
	else // Dimension not active? Activate the 2 second skill block penalty.
		sc_start(src, sd, SC_NOVAEXPLOSING, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case SP_SOULEXPLOSION:
	if (!(tsc && (tsc->getSCE(SC_SPIRIT) || tsc->getSCE(SC_SOULGOLEM) || tsc->getSCE(SC_SOULSHADOW) || tsc->getSCE(SC_SOULFALCON) || tsc->getSCE(SC_SOULFAIRY))) || tstatus->hp < 10 * tstatus->max_hp / 100)
	{ // Requires target to have a soul link and more then 10% of MaxHP.
		// With this skill requiring a soul link, and the target to have more then 10% if MaxHP, I wonder
		// if the cooldown still happens after it fails. Need a confirm. [Rytech]
		if (sd)
			clif_skill_fail(*sd, skill_id);
		break;
	}

	skill_attack(BF_MISC, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case SL_SMA:
	status_change_end(src, SC_SMA);
	[[fallthrough]];
case SL_STIN:
case SL_STUN:
case SP_SPA:
	if (sd && !battle_config.allow_es_magic_pc && bl->type != BL_MOB)
	{
		status_change_start(src, src, SC_STUN, 10000, skill_lv, 0, 0, 0, 500, SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
		clif_skill_fail(*sd, skill_id);
		break;
	}
	skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case NPC_DARKBREATH:
	clif_emotion(*src, ET_ANGER);
	if (rnd() % 2 == 0)
		break; // 50% chance
	[[fallthrough]];
case SN_FALCONASSAULT:
#ifndef RENEWAL
case PA_PRESSURE:
case CR_ACIDDEMONSTRATION:
#endif
case TF_THROWSTONE:
case NPC_SMOKING:
case GS_FLING:
case NJ_ZENYNAGE:
case GN_THORNS_TRAP:
case RL_B_TRAP:
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;
#ifdef RENEWAL
case NJ_ISSEN:
{
	int16 x, y;
	int16 dir = map_calc_dir(src, bl->x, bl->y);

	// Move 2 cells (From target)
	if (dir > 0 && dir < 4)
		x = -2;
	else if (dir > 4)
		x = 2;
	else
		x = 0;
	if (dir > 2 && dir < 6)
		y = -2;
	else if (dir == 7 || dir < 2)
		y = 2;
	else
		y = 0;
	// Doesn't have slide effect in GVG
	if (skill_check_unit_movepos(5, src, bl->x + x, bl->y + y, 1, 1))
	{
		clif_blown(src);
		clif_spiritball(src);
	}
	skill_attack(BF_MISC, src, src, bl, skill_id, skill_lv, tick, flag);
	status_set_hp(src, umax(status_get_max_hp(src) / 100, 1), 0);
	status_change_end(src, SC_NEN);
	status_change_end(src, SC_HIDING);
}
break;
#endif
case RK_DRAGONBREATH_WATER:
case RK_DRAGONBREATH:
case NPC_DRAGONBREATH:
	if (tsc && tsc->getSCE(SC_HIDING))
		clif_skill_nodamage(src, *src, skill_id, skill_lv);
	else
	{
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	}
	break;

case NPC_SELFDESTRUCTION:
	if (tsc && tsc->getSCE(SC_HIDING))
		break;
	[[fallthrough]];
case HVAN_EXPLOSION:
	if (src != bl)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;

// Celest
case PF_SOULBURN:
	if (rnd() % 100 < (skill_lv < 5 ? 30 + skill_lv * 10 : 70))
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		if (skill_lv == 5)
			skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag);
		status_percent_damage(src, bl, 0, 100, false);
	}
	else
	{
		clif_skill_nodamage(src, *src, skill_id, skill_lv);
		if (skill_lv == 5)
			skill_attack(BF_MAGIC, src, src, src, skill_id, skill_lv, tick, flag);
		status_percent_damage(src, src, 0, 100, false);
	}
	break;

case NPC_BLOODDRAIN:
case NPC_ENERGYDRAIN:
{
	int32 heal = (int32)skill_attack((skill_id == NPC_BLOODDRAIN) ? BF_WEAPON : BF_MAGIC,
									 src, src, bl, skill_id, skill_lv, tick, flag);
	if (heal > 0)
	{
		clif_skill_nodamage(nullptr, *src, AL_HEAL, heal);
		status_heal(src, heal, 0, 0);
	}
}
break;

case GS_BULLSEYE:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case NJ_KASUMIKIRI:
	if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag) > 0)
		sc_start(src, src, SC_HIDING, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;
case NJ_KIRIKAGE:
	if (!map_flag_gvg2(src->m) && !map_getmapflag(src->m, MF_BATTLEGROUND))
	{ // You don't move on GVG grounds.
		int16 x, y;
		map_search_freecell(bl, 0, &x, &y, 1, 1, 0);
		if (unit_movepos(src, x, y, 0, 0))
		{
			clif_blown(src);
		}
	}
	status_change_end(src, SC_HIDING);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case RK_PHANTOMTHRUST:
case NPC_PHANTOMTHRUST:
	unit_setdir(src, map_calc_dir(src, bl->x, bl->y));
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);

	skill_blown(src, bl, distance_bl(src, bl) - 1, unit_getdir(src), BLOWN_NONE);
	if (battle_check_target(src, bl, BCT_ENEMY) > 0)
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case RK_WINDCUTTER:
case RK_STORMBLAST:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
	break;
case GC_DARKILLUSION:
{
	int16 x, y;
	int16 dir = map_calc_dir(src, bl->x, bl->y);

	if (dir > 0 && dir < 4)
		x = 2;
	else if (dir > 4)
		x = -2;
	else
		x = 0;
	if (dir > 2 && dir < 6)
		y = 2;
	else if (dir == 7 || dir < 2)
		y = -2;
	else
		y = 0;

	if (unit_movepos(src, bl->x + x, bl->y + y, 1, 1))
	{
		clif_blown(src);
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
		if (rnd() % 100 < 4 * skill_lv)
			skill_castend_damage_id(src, bl, GC_CROSSIMPACT, skill_lv, tick, flag);
	}
}
break;

case GC_CROSSRIPPERSLASHER:
	if (sd && !(sc && sc->getSCE(SC_ROLLINGCUTTER)))
		clif_skill_fail(*sd, skill_id, USESKILL_FAIL_CONDITION);
	else
	{
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	}
	break;
case GC_CROSSIMPACT:
{
	uint8 dir = DIR_NORTHEAST;

	if (bl->x != src->x || bl->y != src->y)
		dir = map_calc_dir(bl, src->x, src->y); // dir based on target as we move player based on target location

	if (skill_check_unit_movepos(0, src, bl->x + dirx[dir], bl->y + diry[dir], 1, 1))
	{
		clif_blown(src);
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	}
	else
	{
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
	}
	break;
}
case GC_PHANTOMMENACE:
	if (flag & 1)
	{ // Only Hits Invisible Targets
		if (tsc && (tsc->option & (OPTION_HIDE | OPTION_CLOAK | OPTION_CHASEWALK) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD)))
		{
			status_change_end(bl, SC_CLOAKINGEXCEED);
			skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
		}
		if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
			status_change_end(bl, SC__SHADOWFORM);															  // Should only end, no damage dealt.
	}
	break;

case GC_DARKCROW:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	sc_start(src, bl, SC_DARKCROW, 100, skill_lv, skill_get_time(skill_id, skill_lv)); // Should be applied even on miss
	break;

case WL_DRAINLIFE:
{
	int32 heal = (int32)skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	int32 rate = 70 + 5 * skill_lv;

	heal = heal * (5 + 5 * skill_lv) / 100;

	if (bl->type == BL_SKILL)
		heal = 0; // Don't absorb heal from Ice Walls or other skill units.

	if (heal && rnd() % 100 < rate)
	{
		status_heal(src, heal, 0, 0);
		clif_skill_nodamage(nullptr, *src, AL_HEAL, heal);
	}
}
break;

case WL_TETRAVORTEX_FIRE:
case WL_TETRAVORTEX_WATER:
case WL_TETRAVORTEX_WIND:
case WL_TETRAVORTEX_GROUND:
	skill_addtimerskill(src, tick + skill_area_temp[0] * 200, bl->id, skill_area_temp[1], 0, skill_id, skill_lv, 0, flag);
	break;

case WL_TETRAVORTEX:
	if (sd == nullptr)
	{ // Monster usage
		uint8 i = 0;
		const static std::vector<std::vector<uint16>> tetra_skills = {{WL_TETRAVORTEX_FIRE, 1},
																	  {WL_TETRAVORTEX_WIND, 4},
																	  {WL_TETRAVORTEX_WATER, 2},
																	  {WL_TETRAVORTEX_GROUND, 8}};

		for (const auto &skill : tetra_skills)
		{
			if (skill_lv > 5)
			{
				skill_area_temp[0] = i;
				skill_area_temp[1] = skill[1];
				map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill[0], skill_lv, tick, flag | BCT_ENEMY, skill_castend_damage_id);
			}
			else
				skill_addtimerskill(src, tick + i * 200, bl->id, skill[1], 0, skill[0], skill_lv, i, flag);
			i++;
		}
	}
	else if (sc)
	{ // No SC? No spheres
		int32 i, k = 0;

		if (sc->getSCE(SC_SPHERE_5)) // If 5 spheres, remove last one (based on reverse order) and only do 4 actions (Official behavior)
			status_change_end(src, SC_SPHERE_1);

		for (i = SC_SPHERE_5; i >= SC_SPHERE_1; i--)
		{ // Loop should always be 4 for regular players, but unconditional_skill could be less
			if (sc->getSCE(static_cast<sc_type>(i)) == nullptr)
				continue;

			uint16 subskill = 0;

			switch (sc->getSCE(static_cast<sc_type>(i))->val1)
			{
			case WLS_FIRE:
				subskill = WL_TETRAVORTEX_FIRE;
				k |= 1;
				break;
			case WLS_WIND:
				subskill = WL_TETRAVORTEX_WIND;
				k |= 4;
				break;
			case WLS_WATER:
				subskill = WL_TETRAVORTEX_WATER;
				k |= 2;
				break;
			case WLS_STONE:
				subskill = WL_TETRAVORTEX_GROUND;
				k |= 8;
				break;
			}

			if (skill_lv > 5)
			{
				skill_area_temp[0] = abs(i - SC_SPHERE_5);
				skill_area_temp[1] = k;
				map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, subskill, skill_lv, tick, flag | BCT_ENEMY, skill_castend_damage_id);
			}
			else
				skill_addtimerskill(src, tick + abs(i - SC_SPHERE_5) * 200, bl->id, k, 0, subskill, skill_lv, abs(i - SC_SPHERE_5), flag);
			status_change_end(src, static_cast<sc_type>(i));
		}
	}
	break;

case WL_RELEASE:
	if (sc == nullptr)
		break;
	if (sd)
	{
		int32 i;

#ifndef RENEWAL
		skill_toggle_magicpower(src, skill_id); // No hit will be amplified
#endif
		if (skill_lv == 1)
		{ // SpellBook
			if (sc->getSCE(SC_FREEZE_SP) == nullptr)
				break;

			bool found_spell = false;

			for (i = SC_MAXSPELLBOOK; i >= SC_SPELLBOOK1; i--)
			{ // List all available spell to be released
				if (sc->getSCE(i) != nullptr)
				{
					found_spell = true;
					break;
				}
			}

			if (!found_spell)
				break;

			// Now extract the data from the preserved spell
			uint16 pres_skill_id = sc->getSCE(i)->val1;
			uint16 pres_skill_lv = sc->getSCE(i)->val2;
			uint16 point = sc->getSCE(i)->val3;

			status_change_end(src, static_cast<sc_type>(i));

			if (sc->getSCE(SC_FREEZE_SP)->val2 > point)
				sc->getSCE(SC_FREEZE_SP)->val2 -= point;
			else // Last spell to be released
				status_change_end(src, SC_FREEZE_SP);

			if (!skill_check_condition_castbegin(*sd, pres_skill_id, pres_skill_lv))
				break;

			// Get the requirement for the preserved skill
			skill_consume_requirement(sd, pres_skill_id, pres_skill_lv, 1);

			switch (skill_get_casttype(pres_skill_id))
			{
			case CAST_GROUND:
				skill_castend_pos2(src, bl->x, bl->y, pres_skill_id, pres_skill_lv, tick, 0);
				break;
			case CAST_NODAMAGE:
				skill_castend_nodamage_id(src, bl, pres_skill_id, pres_skill_lv, tick, 0);
				break;
			case CAST_DAMAGE:
				skill_castend_damage_id(src, bl, pres_skill_id, pres_skill_lv, tick, 0);
				break;
			}

			sd->ud.canact_tick = i64max(tick + skill_delayfix(src, pres_skill_id, pres_skill_lv), sd->ud.canact_tick);
			clif_status_change(src, EFST_POSTDELAY, 1, skill_delayfix(src, pres_skill_id, pres_skill_lv), 0, 0, 0);

			int32 cooldown = pc_get_skillcooldown(sd, pres_skill_id, pres_skill_lv);

			if (cooldown > 0)
				skill_blockpc_start(*sd, pres_skill_id, cooldown);
		}
		else
		{ // Summoned Balls
			for (i = SC_SPHERE_5; i >= SC_SPHERE_1; i--)
			{
				if (sc->getSCE(static_cast<sc_type>(i)) == nullptr)
					continue;

				int32 skele = WL_RELEASE - 5 + sc->getSCE(static_cast<sc_type>(i))->val1 - WLS_FIRE; // Convert Ball Element into Skill ATK for balls

				// WL_SUMMON_ATK_FIRE, WL_SUMMON_ATK_WIND, WL_SUMMON_ATK_WATER, WL_SUMMON_ATK_GROUND
				skill_addtimerskill(src, tick + (t_tick)status_get_adelay(src) * abs(i - SC_SPHERE_1), bl->id, 0, 0, skele, sc->getSCE(static_cast<sc_type>(i))->val2, BF_MAGIC, flag | SD_LEVEL);
				status_change_end(src, static_cast<sc_type>(i)); // Eliminate ball
			}
			clif_skill_nodamage(src, *bl, skill_id, 0);
		}
	}
	break;
case WL_FROSTMISTY:
	// Causes Freezing status through walls.
	sc_start(src, bl, SC_FREEZING, 25 + 5 * skill_lv, skill_lv, skill_get_time(skill_id, skill_lv));
	sc_start(src, bl, SC_MISTY_FROST, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	// Doesn't deal damage through non-shootable walls.
	if (!battle_config.skill_wall_check || (battle_config.skill_wall_check && path_search(nullptr, src->m, src->x, src->y, bl->x, bl->y, 1, CELL_CHKWALL)))
		skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag | SD_ANIMATION);
	break;
case WL_HELLINFERNO:
	if (flag & 1)
	{
		skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag);
		skill_addtimerskill(src, tick + 300, bl->id, 0, 0, skill_id, skill_lv, BF_MAGIC, flag | 2);
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
	break;
case RA_WUGSTRIKE:
	if (sd && pc_isridingwug(sd))
	{
		uint8 dir = map_calc_dir(bl, src->x, src->y);

		if (unit_movepos(src, bl->x + dirx[dir], bl->y + diry[dir], 1, 1))
		{
			clif_blown(src);
			skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
		}
		break;
	}
	[[fallthrough]];
case RA_WUGBITE:
	if (path_search(nullptr, src->m, src->x, src->y, bl->x, bl->y, 1, CELL_CHKNOREACH))
	{
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	}
	else if (sd && skill_id == RA_WUGBITE) // Only RA_WUGBITE has the skill fail message.
		clif_skill_fail(*sd, skill_id);

	break;

case RA_SENSITIVEKEEN:
	if (bl->type != BL_SKILL)
	{ // Only Hits Invisible Targets
		if (tsc && ((tsc->option & (OPTION_HIDE | OPTION_CLOAK | OPTION_CHASEWALK)) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD)))
		{
			status_change_end(bl, SC_CLOAKINGEXCEED);
			skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
		}
		if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
			status_change_end(bl, SC__SHADOWFORM);															  // Should only end, no damage dealt.
	}
	else
	{
		skill_unit *su = BL_CAST(BL_SKILL, bl);
		std::shared_ptr<s_skill_unit_group> sg;

		if (su && (sg = su->group) && skill_get_inf2(sg->skill_id, INF2_ISTRAP))
		{
			if (!(sg->unit_id == UNT_USED_TRAPS || (sg->unit_id == UNT_ANKLESNARE && sg->val2 != 0)))
			{
				struct item item_tmp;
				memset(&item_tmp, 0, sizeof(item_tmp));
				item_tmp.nameid = sg->item_id ? sg->item_id : ITEMID_TRAP;
				item_tmp.identify = 1;
				if (item_tmp.nameid)
					map_addflooritem(&item_tmp, 1, bl->m, bl->x, bl->y, 0, 0, 0, 4, 0);
			}
			skill_delunit(su);
		}
	}
	break;
case NC_INFRAREDSCAN:
	if (flag & 1)
	{
		status_change_end(bl, SC_HIDING);
		status_change_end(bl, SC_CLOAKING);
		status_change_end(bl, SC_CLOAKINGEXCEED);
		status_change_end(bl, SC_CAMOUFLAGE);
		status_change_end(bl, SC_NEWMOON);
		if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
			status_change_end(bl, SC__SHADOWFORM);
		sc_start(src, bl, SC_INFRAREDSCAN, 10000, skill_lv, skill_get_time(skill_id, skill_lv));
	}
	else
	{
		clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), splash_target(src), src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
	break;
case SC_FATALMENACE:
	if (flag & 1)
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	else
	{
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), splash_target(src), src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
		clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	}
	break;

case NPC_FATALMENACE: // todo should it teleport the target ?
	if (flag & 1)
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	else
	{
		int16 x, y;
		map_search_freecell(src, 0, &x, &y, -1, -1, 0);
		// Destination area
		skill_area_temp[4] = x;
		skill_area_temp[5] = y;
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), splash_target(src), src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
		skill_addtimerskill(src, tick + 800, src->id, x, y, skill_id, skill_lv, 0, flag); // To teleport Self
		clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	}
	break;

case LG_PINPOINTATTACK:
	if (skill_check_unit_movepos(5, src, bl->x, bl->y, 1, 1))
		clif_blown(src);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case SR_KNUCKLEARROW:
	// Holds current direction of bl/target to src/attacker before the src is moved to bl location
	dir_ka = map_calc_dir(bl, src->x, src->y);
	// Has slide effect
	if (skill_check_unit_movepos(5, src, bl->x, bl->y, 1, 1))
		skill_blown(src, src, 1, (dir_ka + 4) % 8, BLOWN_NONE); // Target position is actually one cell next to the target
	skill_addtimerskill(src, tick + 300, bl->id, 0, 0, skill_id, skill_lv, BF_WEAPON, flag | SD_LEVEL | 2);
	break;

case SR_HOWLINGOFLION:
	status_change_end(bl, SC_SWINGDANCE);
	status_change_end(bl, SC_SYMPHONYOFLOVER);
	status_change_end(bl, SC_MOONLITSERENADE);
	status_change_end(bl, SC_RUSHWINDMILL);
	status_change_end(bl, SC_ECHOSONG);
	status_change_end(bl, SC_HARMONIZE);
	status_change_end(bl, SC_NETHERWORLD);
	status_change_end(bl, SC_VOICEOFSIREN);
	status_change_end(bl, SC_DEEPSLEEP);
	status_change_end(bl, SC_SIRCLEOFNATURE);
	status_change_end(bl, SC_GLOOMYDAY);
	status_change_end(bl, SC_GLOOMYDAY_SK);
	status_change_end(bl, SC_SONGOFMANA);
	status_change_end(bl, SC_DANCEWITHWUG);
	status_change_end(bl, SC_SATURDAYNIGHTFEVER);
	status_change_end(bl, SC_LERADSDEW);
	status_change_end(bl, SC_MELODYOFSINK);
	status_change_end(bl, SC_BEYONDOFWARCRY);
	status_change_end(bl, SC_UNLIMITEDHUMMINGVOICE);
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag | SD_ANIMATION);
	break;

case SR_EARTHSHAKER:
	if (flag & 1)
	{ // by default cloaking skills are remove by aoe skills so no more checking/removing except hiding and cloaking exceed.
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
		status_change_end(bl, SC_CLOAKINGEXCEED);
		if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
			status_change_end(bl, SC__SHADOWFORM);
	}
	else
	{
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	}
	break;

case SR_TIGERCANNON:
	if (flag & 1)
	{
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag | SD_ANIMATION);
	}
	else if (sd)
	{
		if (sc && sc->getSCE(SC_COMBO) && sc->getSCE(SC_COMBO)->val1 == SR_FALLENEMPIRE && !sc->getSCE(SC_FLASHCOMBO))
			flag |= 8; // Only apply Combo bonus when Tiger Cannon is not used through Flash Combo
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
	break;

case WM_REVERBERATION:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinallrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		battle_consume_ammo(sd, skill_id, skill_lv); // Consume here since Magic/Misc attacks reset arrow_atk
	}
	break;

case NPC_POISON_BUSTER:
	if (tsc && tsc->getSCE(SC_POISON))
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
		status_change_end(bl, SC_POISON);
	}
	else if (sd)
		clif_skill_fail(*sd, skill_id);
	break;

case KO_JYUMONJIKIRI:
{
	int16 x, y;
	int16 dir = map_calc_dir(src, bl->x, bl->y);

	if (dir > 0 && dir < 4)
		x = 2;
	else if (dir > 4)
		x = -2;
	else
		x = 0;
	if (dir > 2 && dir < 6)
		y = 2;
	else if (dir == 7 || dir < 2)
		y = -2;
	else
		y = 0;
	if (unit_movepos(src, bl->x + x, bl->y + y, 1, 1))
	{
		clif_blown(src);
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	}
}
break;

case EL_FIRE_BOMB:
case EL_FIRE_WAVE:
case EL_WATER_SCREW:
case EL_HURRICANE:
case EL_TYPOON_MIS:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id + 1), src, src, bl, skill_id + 1, skill_lv, tick, flag);
	else
	{
		int32 i = skill_get_splash(skill_id, skill_lv);
		clif_skill_nodamage(src, *battle_get_master(src), skill_id, skill_lv);
		clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
		if (rnd() % 100 < 30)
			map_foreachinrange(skill_area_sub, bl, i, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
		else
			skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	}
	break;

case EL_ROCK_CRUSHER:
	clif_skill_nodamage(src, *battle_get_master(src), skill_id, skill_lv);
	clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	if (rnd() % 100 < 50)
		skill_attack(BF_MAGIC, src, src, bl, skill_id, skill_lv, tick, flag);
	else
		skill_attack(BF_WEAPON, src, src, bl, EL_ROCK_CRUSHER_ATK, skill_lv, tick, flag);
	break;

case EL_STONE_RAIN:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	else
	{
		int32 i = skill_get_splash(skill_id, skill_lv);
		clif_skill_nodamage(src, *battle_get_master(src), skill_id, skill_lv);
		clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
		if (rnd() % 100 < 30)
			map_foreachinrange(skill_area_sub, bl, i, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
		else
			skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	}
	break;

case EL_FIRE_ARROW:
case EL_ICE_NEEDLE:
case EL_WIND_SLASH:
case EL_STONE_HAMMER:
	clif_skill_nodamage(src, *battle_get_master(src), skill_id, skill_lv);
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case EL_TIDAL_WEAPON:
	if (src->type == BL_ELEM)
	{
		s_elemental_data *ele = BL_CAST(BL_ELEM, src);
		status_change *tsc_ele = status_get_sc(ele);
		sc_type type = SC_TIDAL_WEAPON_OPTION, type2 = SC_TIDAL_WEAPON;

		clif_skill_nodamage(src, *battle_get_master(src), skill_id, skill_lv);
		clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, skill_id, skill_lv, DMG_SINGLE);
		if ((tsc_ele && tsc_ele->getSCE(type2)) || (tsc && tsc->getSCE(type)))
		{
			status_change_end(battle_get_master(src), type);
			status_change_end(src, type2);
		}
		if (rnd() % 100 < 50)
			skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
		else
		{
			sc_start(src, src, type2, 100, skill_lv, skill_get_time(skill_id, skill_lv));
			sc_start(src, battle_get_master(src), type, 100, ele->id, skill_get_time(skill_id, skill_lv));
		}
		clif_skill_nodamage(src, *src, skill_id, skill_lv);
	}
	break;

// recursive homon skill
case MH_ABSOLUTE_ZEPHYR:
case MH_TOXIN_OF_MANDARA:
case MH_BLAZING_AND_FURIOUS:
case MH_MAGMA_FLOW:
case MH_HEILIGE_STANGE:
	if (flag & 1)
	{
		if ((skill_id == MH_MAGMA_FLOW) && ((rnd() % 100) > (3 * skill_lv)))
			break; // chance to not trigger atk for magma
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	}
	else
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	break;

case MH_TWISTER_CUTTER:
case MH_GLANZEN_SPIES:
case MH_STAHL_HORN:
case MH_NEEDLE_OF_PARALYZE:
case MH_NEEDLE_STINGER:
case MH_SONIC_CRAW:
case MH_MIDNIGHT_FRENZY:
case MH_SILVERVEIN_RUSH:
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case MH_TINDER_BREAKER:
case MH_CBC:
case MH_EQC:
{
	TBL_HOM *hd = BL_CAST(BL_HOM, src);
	int32 duration = max(skill_lv, (status_get_str(src) / 7 - status_get_str(bl) / 10)) * 1000; // Yommy formula
	sc_type type;

	if (skill_id == MH_TINDER_BREAKER)
	{
		type = SC_TINDER_BREAKER2;
		if (unit_movepos(src, bl->x, bl->y, 1, 1))
		{
			clif_blown(src);
			clif_skill_poseffect(*src, skill_id, skill_lv, bl->x, bl->y, tick);
		}
	}
	else if (skill_id == MH_CBC)
	{
		type = SC_CBC;
	}
	else if (skill_id == MH_EQC)
	{
		type = SC_EQC;
	}

	clif_skill_nodamage(src, *bl, skill_id, skill_lv, sc_start4(src, bl, type, 100, skill_lv, src->id, 0, 0, duration));
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
}
break;

case RL_H_MINE:
	if (!(flag & 1))
	{
		// Direct attack
		if (!sd || !sd->flicker)
		{
			if (skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag))
				status_change_start(src, bl, SC_H_MINE, 10000, skill_id, 0, 0, 0, skill_get_time(skill_id, skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
			break;
		}
		// Triggered by RL_FLICKER
		if (sd && sd->flicker)
		{
			// Splash damage around it!
			map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR | BL_SKILL,
							   src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
			flag |= 1; // Don't consume requirement
			if (tsc && tsc->getSCE(SC_H_MINE) && tsc->getSCE(SC_H_MINE)->val2 == src->id)
			{
				status_change_end(bl, SC_H_MINE);
				sc_start4(src, bl, SC_BURNING, 10 * skill_lv, skill_lv, 1000, src->id, 0, skill_get_time2(skill_id, skill_lv));
			}
		}
	}
	else
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	if (sd && sd->flicker)
		flag |= 1; // Don't consume requirement
	break;

case RL_QD_SHOT:
	// Except for main target, only units marked with crimson marker are valid targets
	if (skill_area_temp[1] == bl->id || (tsc != nullptr && tsc->getSCE(SC_C_MARKER) != nullptr))
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case RL_D_TAIL:
case RL_HAMMER_OF_GOD:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag | SD_ANIMATION);
	else
	{
		if (sd && tsc && tsc->getSCE(SC_C_MARKER))
		{
			int32 i;

			ARR_FIND(0, MAX_SKILL_CRIMSON_MARKER, i, sd->c_marker[i] == bl->id);

			if (i < MAX_SKILL_CRIMSON_MARKER)
				flag |= 8;
		}

		if (skill_id == RL_HAMMER_OF_GOD)
			clif_skill_poseffect(*src, skill_id, 1, bl->x, bl->y, gettick());
		else
			clif_skill_nodamage(src, *bl, skill_id, skill_lv);

		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
	break;

case SU_SCAROFTAROU:
	sc_start(src, bl, SC_BITESCAR, 10, skill_lv, skill_get_time(skill_id, skill_lv)); //! TODO: What's the activation chance for the Bite effect?
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;
case SU_SV_STEMSPEAR:
	if (sd && pc_checkskill(sd, SU_SPIRITOFLAND))
		sc_start(src, src, SC_DORAM_WALKSPEED, 100, 50, skill_get_time(SU_SPIRITOFLAND, 1));
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case 0: /* no skill - basic/normal attack */
	if (sd)
	{
		if (flag & 3)
		{
			if (bl->id != skill_area_temp[1])
				skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, SD_LEVEL | flag);
		}
		else
		{
			skill_area_temp[1] = bl->id;
			map_foreachinallrange(skill_area_sub, bl,
								  sd->bonus.splash_range, BL_CHAR,
								  src, skill_id, skill_lv, tick, flag | BCT_ENEMY | 1,
								  skill_castend_damage_id);
			flag |= 1; // Set flag to 1 so ammo is not double-consumed. [Skotlex]
		}
	}
	break;

case SJ_FALLINGSTAR_ATK:
	if (sd)
	{ // If a player used the skill it will search for targets marked by that player.
		if (tsc && tsc->getSCE(SC_FLASHKICK) && tsc->getSCE(SC_FLASHKICK)->val4 == 1)
		{ // Mark placed by a player.
			int8 i = 0;

			ARR_FIND(0, MAX_STELLAR_MARKS, i, sd->stellar_mark[i] == bl->id);
			if (i < MAX_STELLAR_MARKS)
			{
				skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
				skill_castend_damage_id(src, bl, SJ_FALLINGSTAR_ATK2, skill_lv, tick, 0);
			}
		}
	}
	else if (tsc && tsc->getSCE(SC_FLASHKICK) && tsc->getSCE(SC_FLASHKICK)->val4 == 2)
	{ // Mark placed by a monster.
		// If a monster used the skill it will search for targets marked by any monster since they can't track their own targets.
		skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
		skill_castend_damage_id(src, bl, SJ_FALLINGSTAR_ATK2, skill_lv, tick, 0);
	}
	break;
case SJ_FLASHKICK:
{
	map_session_data *tsd = BL_CAST(BL_PC, bl);
	struct mob_data *md = BL_CAST(BL_MOB, src), *tmd = BL_CAST(BL_MOB, bl);

	// Only players and monsters can be tagged....I think??? [Rytech]
	// Lets only allow players and monsters to use this skill for safety reasons.
	if ((!tsd && !tmd) || !sd && !md)
	{
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
		break;
	}

	// Check if the target is already tagged by another source.
	if ((tsd && tsd->sc.getSCE(SC_FLASHKICK) && tsd->sc.getSCE(SC_FLASHKICK)->val1 != src->id) || (tmd && tmd->sc.getSCE(SC_FLASHKICK) && tmd->sc.getSCE(SC_FLASHKICK)->val1 != src->id))
	{ // Same as the above check, but for monsters.
		// Can't tag a player that was already tagged from another source.
		if (sd)
			clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
		map_freeblock_unlock();
		return 1;
	}

	if (sd)
	{ // Tagging the target.
		int32 i;

		ARR_FIND(0, MAX_STELLAR_MARKS, i, sd->stellar_mark[i] == bl->id);
		if (i == MAX_STELLAR_MARKS)
		{
			ARR_FIND(0, MAX_STELLAR_MARKS, i, sd->stellar_mark[i] == 0);
			if (i == MAX_STELLAR_MARKS)
			{ // Max number of targets tagged. Fail the skill.
				clif_skill_fail(*sd, skill_id, USESKILL_FAIL);
				map_freeblock_unlock();
				return 1;
			}
		}

		// Tag the target only if damage was done. If it deals no damage, it counts as a miss and won't tag.
		// Note: Not sure if it works like this in official but you can't mark on something you can't
		// hit, right? For now well just use this logic until we can get a confirm on if it does this or not. [Rytech]
		if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag) > 0)
		{ // Add the ID of the tagged target to the player's tag list and start the status on the target.
			sd->stellar_mark[i] = bl->id;

			// Val4 flags if the status was applied by a player or a monster.
			// This will be important for other skills that work together with this one.
			// 1 = Player, 2 = Monster.
			// Note: Because the attacker's ID and the slot number is handled here, we have to
			// apply the status here. We can't pass this data to skill_additional_effect.
			sc_start4(src, bl, SC_FLASHKICK, 100, src->id, i, skill_lv, 1, skill_get_time(skill_id, skill_lv));
		}
	}
	else if (md)
	{ // Monsters can't track with this skill. Just give the status.
		if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag) > 0)
			sc_start4(src, bl, SC_FLASHKICK, 100, 0, 0, skill_lv, 2, skill_get_time(skill_id, skill_lv));
	}
}
break;

case NPC_VENOMIMPRESS:
	if (skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag))
		sc_start(src, bl, SC_VENOMIMPRESS, 100, skill_lv, skill_get_time(skill_id, skill_lv));
	break;

case HN_DOUBLEBOWLINGBASH:
	if (flag & 1)
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, skill_area_temp[0] & 0xFFF);
	}
	else
	{
		int32 splash = skill_get_splash(skill_id, skill_lv);
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		skill_area_temp[0] = map_foreachinallrange(skill_area_sub, bl, splash, BL_CHAR, src, skill_id, skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
		map_foreachinrange(skill_area_sub, bl, splash, BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		sc_start(src, src, SC_HNNOWEAPON, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	}
	break;

case HN_SHIELD_CHAIN_RUSH:
	if (flag & 1)
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		sc_start(src, src, SC_HNNOWEAPON, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
	}
	break;

case HN_JACK_FROST_NOVA:
case HN_HELLS_DRIVE:
case HN_GROUND_GRAVITATION:
	if (flag & 1)
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	break;

case HN_NAPALM_VULCAN_STRIKE:
	if (flag & 1)
	{
		skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	}
	else
	{
		clif_skill_nodamage(src, *bl, skill_id, skill_lv);
		map_foreachinrange(skill_area_sub, bl, skill_get_splash(skill_id, skill_lv), BL_CHAR, src, skill_id, skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
	break;

case NW_MAGAZINE_FOR_ONE:
case NW_ONLY_ONE_BULLET:
	skill_attack(BF_WEAPON, src, src, bl, skill_id, skill_lv, tick, flag);
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
	break;

case SOA_TALISMAN_OF_BLUE_DRAGON:
	clif_skill_nodamage(src, *bl, skill_id, skill_lv);
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	sc_start(src, src, skill_get_sc(skill_id), 100, 1, skill_get_time(skill_id, skill_lv));
	break;

case SOA_TALISMAN_OF_SOUL_STEALING:
	skill_attack(skill_get_type(skill_id), src, src, bl, skill_id, skill_lv, tick, flag);
	if (bl->type != BL_SKILL)
	{
		int32 sp = (100 + status_get_lv(src) / 50) * skill_lv;

		status_heal(src, 0, sp, 0, 0);
		clif_skill_nodamage(src, *src, skill_id, sp);
	}
	break;

default:
	if (std::shared_ptr<s_skill_db> skill = skill_db.find(skill_id); skill != nullptr && skill->impl != nullptr)
	{
		skill->impl->castendDamageId(src, bl, skill_lv, tick, flag);
		break;
	}

	ShowWarning("skill_castend_damage_id: Unknown skill used:%d\n", skill_id);
	clif_skill_damage(*src, *bl, tick, status_get_amotion(src), tstatus->dmotion,
					  0, abs(skill_get_num(skill_id, skill_lv)),
					  skill_id, skill_lv, skill_get_hit(skill_id));
	map_freeblock_unlock();
	return 1;
}

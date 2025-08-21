switch (skill_id)
{
case SM_MAGNUM:
case MS_MAGNUM:
	if (wd->miscflag == 1)
		skillratio += 20 * skill_lv; // Inner 3x3 circle takes 100%+20%*level damage [Playtester]
	else
		skillratio += 10 * skill_lv; // Outer 5x5 circle takes 100%+10%*level damage [Playtester]
	break;
case MC_MAMMONITE:
	skillratio += 50 * skill_lv;
	break;
case HT_POWER:
	skillratio += -50 + 8 * sstatus->str;
	break;
case AC_DOUBLE:
case MA_DOUBLE:
	skillratio += 10 * (skill_lv - 1);
	break;
case AC_SHOWER:
case MA_SHOWER:
#ifdef RENEWAL
	skillratio += 50 + 10 * skill_lv;
#else
	skillratio += -25 + 5 * skill_lv;
#endif
	break;
case AC_CHARGEARROW:
case MA_CHARGEARROW:
	skillratio += 50;
	break;
case KN_PIERCE:
	skillratio += 10 * skill_lv;
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		skillratio *= 2;
	break;
case ML_PIERCE:
	skillratio += 10 * skill_lv;
	break;
case MER_CRASH:
	skillratio += 10 * skill_lv;
	break;
case KN_SPEARSTAB:
	skillratio += 20 * skill_lv;
	break;
case KN_SPEARBOOMERANG:
	skillratio += 50 * skill_lv;
	break;
#ifdef RENEWAL
case KN_BRANDISHSPEAR:
	skillratio += -100 + 400 + 100 * skill_lv + sstatus->str * 3;
	break;
#else
case KN_BRANDISHSPEAR:
#endif
case ML_BRANDISH:
{
	int32 ratio = 100 + 20 * skill_lv;

	skillratio += -100 + ratio;
	if (skill_lv > 3 && wd->miscflag == 0)
		skillratio += ratio / 2;
	if (skill_lv > 6 && wd->miscflag == 0)
		skillratio += ratio / 4;
	if (skill_lv > 9 && wd->miscflag == 0)
		skillratio += ratio / 8;
	if (skill_lv > 6 && wd->miscflag == 1)
		skillratio += ratio / 2;
	if (skill_lv > 9 && wd->miscflag == 1)
		skillratio += ratio / 4;
	if (skill_lv > 9 && wd->miscflag == 2)
		skillratio += ratio / 2;
}
break;
case KN_BOWLINGBASH:
case MS_BOWLINGBASH:
	skillratio += 40 * skill_lv;
	break;
case AS_GRIMTOOTH:
	skillratio += 20 * skill_lv;
	break;
case AS_SONICBLOW:
#ifdef RENEWAL
	skillratio += 100 + 100 * skill_lv;
	if (tstatus->hp < (tstatus->max_hp / 2))
		skillratio += skillratio / 2;
#else
	skillratio += 200 + 50 * skill_lv;
	if (sd && pc_checkskill(sd, AS_SONICACCEL) > 0)
		skillratio += skillratio / 10;
#endif
	break;
case TF_SPRINKLESAND:
	skillratio += 30;
	break;
case MC_CARTREVOLUTION:
	skillratio += 50;
	if (sd && sd->cart_weight)
		skillratio += 100 * sd->cart_weight / sd->cart_weight_max; // +1% every 1% weight
	else if (!sd)
		skillratio += 100; // Max damage for non players.
	break;
case NPC_PIERCINGATT:
	skillratio += -25; // 75% base damage
	break;
case NPC_COMBOATTACK:
	skillratio += 25 * skill_lv;
	break;
case NPC_RANDOMATTACK:
case NPC_WATERATTACK:
case NPC_GROUNDATTACK:
case NPC_FIREATTACK:
case NPC_WINDATTACK:
case NPC_POISONATTACK:
case NPC_HOLYATTACK:
case NPC_DARKNESSATTACK:
case NPC_UNDEADATTACK:
case NPC_TELEKINESISATTACK:
case NPC_BLOODDRAIN:
case NPC_ACIDBREATH:
case NPC_DARKNESSBREATH:
case NPC_FIREBREATH:
case NPC_ICEBREATH:
case NPC_ICEBREATH2:
case NPC_THUNDERBREATH:
case NPC_HELLJUDGEMENT:
case NPC_HELLJUDGEMENT2:
case NPC_PULSESTRIKE:
	skillratio += 100 * (skill_lv - 1);
	break;
case NPC_REVERBERATION_ATK:
	skillratio += 400 + 200 * skill_lv;
	break;
case RG_BACKSTAP:
	if (sd && sd->status.weapon == W_BOW && battle_config.backstab_bow_penalty)
		skillratio += (200 + 40 * skill_lv) / 2;
	else
		skillratio += 200 + 40 * skill_lv;
	break;
case RG_RAID:
#ifdef RENEWAL
	skillratio += -100 + 50 + skill_lv * 150;
#else
	skillratio += 40 * skill_lv;
#endif
	break;
case RG_INTIMIDATE:
	skillratio += 30 * skill_lv;
	break;
case CR_SHIELDCHARGE:
	skillratio += 20 * skill_lv;
	break;
case CR_SHIELDBOOMERANG:
#ifdef RENEWAL
	skillratio += -100 + skill_lv * 80;
#else
	skillratio += 30 * skill_lv;
#endif
	break;
case NPC_DARKCROSS:
case CR_HOLYCROSS:
#ifdef RENEWAL
	if (sd && sd->status.weapon == W_2HSPEAR)
		skillratio += 70 * skill_lv;
	else
#endif
		skillratio += 35 * skill_lv;
	break;
case AM_DEMONSTRATION:
	skillratio += 20 * skill_lv;
	break;
case AM_ACIDTERROR:
#ifdef RENEWAL
	skillratio += -100 + 200 * skill_lv;
	if (sd && pc_checkskill(sd, AM_LEARNINGPOTION))
		skillratio += 100; // !TODO: What's this bonus increase?
#else
	skillratio += -50 + 50 * skill_lv;
#endif
	break;
case MO_FINGEROFFENSIVE:
#ifdef RENEWAL
	skillratio += 500 + skill_lv * 200;
	if (tsc && tsc->getSCE(SC_BLADESTOP))
		skillratio += skillratio / 2;
#else
	skillratio += 50 * skill_lv;
#endif
	break;
case MO_INVESTIGATE:
#ifdef RENEWAL
	skillratio += -100 + 100 * skill_lv;
	if (tsc && tsc->getSCE(SC_BLADESTOP))
		skillratio += skillratio / 2;
#else
	skillratio += 75 * skill_lv;
#endif
	break;
case MO_EXTREMITYFIST:
	skillratio += 700 + sstatus->sp * 10;
#ifdef RENEWAL
	if (wd->miscflag & 1)
		skillratio *= 2; // More than 5 spirit balls active
#endif
	skillratio = min(500000, skillratio); // We stop at roughly 50k SP for overflow protection
	break;
case MO_TRIPLEATTACK:
	skillratio += 20 * skill_lv;
	break;
case MO_CHAINCOMBO:
#ifdef RENEWAL
	skillratio += 150 + 50 * skill_lv;
	if (sd && sd->status.weapon == W_KNUCKLE)
		skillratio *= 2;
#else
	skillratio += 50 + 50 * skill_lv;
#endif
	break;
case MO_COMBOFINISH:
#ifdef RENEWAL
	skillratio += 450 + 50 * skill_lv + sstatus->str; // !TODO: How does STR play a role?
#else
	skillratio += 140 + 60 * skill_lv;
#endif
	if (sc->getSCE(SC_GT_ENERGYGAIN))
		skillratio += skillratio * 50 / 100;
	break;
case BA_MUSICALSTRIKE:
case DC_THROWARROW:
#ifdef RENEWAL
	skillratio += 10 + 40 * skill_lv;
#else
	skillratio += 25 + 25 * skill_lv;
#endif
	break;
case CH_TIGERFIST:
#ifdef RENEWAL
	skillratio += 400 + 150 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += -60 + 100 * skill_lv;
#endif
	if (sc->getSCE(SC_GT_ENERGYGAIN))
		skillratio += skillratio * 50 / 100;
	break;
case CH_CHAINCRUSH:
#ifdef RENEWAL
	skillratio += -100 + 200 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += 300 + 100 * skill_lv;
#endif
	if (sc->getSCE(SC_GT_ENERGYGAIN))
		skillratio += skillratio * 50 / 100;
	break;
case CH_PALMSTRIKE:
#ifdef RENEWAL
	skillratio += 100 + 100 * skill_lv + sstatus->str; // !TODO: How does STR play a role?
	RE_LVL_DMOD(100);
#else
	skillratio += 100 + 100 * skill_lv;
#endif
	break;
case LK_HEADCRUSH:
	skillratio += 40 * skill_lv;
	break;
case LK_JOINTBEAT:
	skillratio += 10 * skill_lv - 50;
	if (wd->miscflag & BREAK_NECK || (tsc && tsc->getSCE(SC_JOINTBEAT) && tsc->getSCE(SC_JOINTBEAT)->val2 & BREAK_NECK)) // The 2x damage is only for the BREAK_NECK ailment.
		skillratio *= 2;
	break;
#ifdef RENEWAL
// Renewal: skill ratio applies to entire damage [helvetica]
case LK_SPIRALPIERCE:
	skillratio += 50 + 50 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		skillratio *= 2;
	break;
case ML_SPIRALPIERCE:
	skillratio += 50 + 50 * skill_lv;
	RE_LVL_DMOD(100);
	break;
#endif
case ASC_METEORASSAULT:
#ifdef RENEWAL
	skillratio += 100 + 120 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += -60 + 40 * skill_lv;
#endif
	break;
case SN_SHARPSHOOTING:
	if (src->type == BL_MOB)
	{						  // TODO: Did these formulas change in the renewal balancing?
		if (wd->miscflag & 2) // Splash damage bonus
			skillratio += -100 + 140 * skill_lv;
		else
			skillratio += 100 + 50 * skill_lv;
		break;
	}
	[[fallthrough]];
case MA_SHARPSHOOTING:
#ifdef RENEWAL
	skillratio += -100 + 300 + 300 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += 100 + 50 * skill_lv;
#endif
	break;
#ifdef RENEWAL
case CR_ACIDDEMONSTRATION:
	skillratio += -100 + 200 * skill_lv + sstatus->int_ + tstatus->vit; // !TODO: Confirm status bonus
	if (target->type == BL_PC)
		skillratio /= 2;
	break;
#endif
case CG_ARROWVULCAN:
#ifdef RENEWAL
	skillratio += 400 + 100 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += 100 + 100 * skill_lv;
#endif
	break;
case AS_SPLASHER:
#ifdef RENEWAL
	skillratio += -100 + 400 + 100 * skill_lv;
#else
	skillratio += 400 + 50 * skill_lv;
#endif
	if (sd)
		skillratio += 20 * pc_checkskill(sd, AS_POISONREACT);
	break;
case ASC_BREAKER:
#ifdef RENEWAL
	skillratio += -100 + 150 * skill_lv + sstatus->str + sstatus->int_; // !TODO: Confirm stat modifier
	RE_LVL_DMOD(100);
#else
	// Pre-Renewal: skill ratio for weapon part of damage [helvetica]
	skillratio += -100 + 100 * skill_lv;
#endif
	break;
case PA_SACRIFICE:
	skillratio += -10 + 10 * skill_lv;
	break;
case PA_SHIELDCHAIN:
#ifdef RENEWAL
	skillratio = -100 + 300 + 200 * skill_lv;

	if (sd != nullptr)
	{
		int16 index = sd->equip_index[EQI_HAND_L];

		// Damage affected by the shield's weight and refine.
		if (index >= 0 && sd->inventory_data[index] != nullptr && sd->inventory_data[index]->type == IT_ARMOR)
		{
			skillratio += sd->inventory_data[index]->weight / 10 + 4 * sd->inventory.u.items_inventory[index].refine;
		}

		// Damage affected by shield mastery
		if (sc != nullptr && sc->getSCE(SC_SHIELD_POWER))
		{
			skillratio += skill_lv * 14 * pc_checkskill(sd, IG_SHIELD_MASTERY);
		}
	}

	RE_LVL_DMOD(100);
#else
	skillratio += 30 * skill_lv;
#endif
	if (sc && sc->getSCE(SC_SHIELD_POWER)) // Whats the official increase? [Rytech]
		skillratio += skillratio * 50 / 100;
	break;
case WS_CARTTERMINATION:
	i = 10 * (16 - skill_lv);
	if (i < 1)
		i = 1;
	// Preserve damage ratio when max cart weight is changed.
	if (sd && sd->cart_weight)
		skillratio += sd->cart_weight / i * 80000 / battle_config.max_cart_weight - 100;
	else if (!sd)
		skillratio += 80000 / i - 100;
	break;
case TK_DOWNKICK:
case TK_STORMKICK:
	skillratio += 60 + 20 * skill_lv;
	break;
case TK_TURNKICK:
case TK_COUNTER:
	skillratio += 90 + 30 * skill_lv;
	break;
case TK_JUMPKICK:
	// Different damage formulas depending on damage trigger
	if (sc && sc->getSCE(SC_COMBO) && sc->getSCE(SC_COMBO)->val1 == skill_id)
		skillratio += -100 + 4 * status_get_lv(src); // Tumble formula [4%*baselevel]
	else if (wd->miscflag)
	{
		skillratio += -100 + 4 * status_get_lv(src); // Running formula [4%*baselevel]
		if (sc && sc->getSCE(SC_SPURT))				 // Spurt formula [8%*baselevel]
			skillratio *= 2;
	}
	else
		skillratio += -70 + 10 * skill_lv;
	break;
case GS_TRIPLEACTION:
	skillratio += 50 * skill_lv;
	break;
case GS_BULLSEYE:
	// Only works well against brute/demihumans non bosses.
	if ((tstatus->race == RC_BRUTE || tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_PLAYER_HUMAN || tstatus->race == RC_PLAYER_DORAM) && !status_has_mode(tstatus, MD_STATUSIMMUNE))
		skillratio += 400;
	break;
case GS_TRACKING:
	skillratio += 100 * (skill_lv + 1);
	break;
case GS_PIERCINGSHOT:
#ifdef RENEWAL
	if (sd && sd->weapontype1 == W_RIFLE)
		skillratio += 150 + 30 * skill_lv;
	else
		skillratio += 100 + 20 * skill_lv;
#else
	skillratio += 20 * skill_lv;
#endif
	break;
case GS_RAPIDSHOWER:
	skillratio += 400 + 50 * skill_lv;
	break;
case GS_DESPERADO:
	skillratio += 50 * (skill_lv - 1);
	if (sc && sc->getSCE(SC_FALLEN_ANGEL))
		skillratio *= 2;
	break;
case GS_DUST:
	skillratio += 50 * skill_lv;
	break;
case GS_FULLBUSTER:
	skillratio += 100 * (skill_lv + 2);
	break;
case GS_SPREADATTACK:
#ifdef RENEWAL
	skillratio += 30 * skill_lv;
#else
	skillratio += 20 * (skill_lv - 1);
#endif
	break;
#ifdef RENEWAL
case GS_GROUNDDRIFT:
	skillratio += 100 + 20 * skill_lv;
	break;
#endif
case NJ_HUUMA:
#ifdef RENEWAL
	skillratio += -150 + 250 * skill_lv;
#else
	skillratio += 50 + 150 * skill_lv;
#endif
	break;
case NJ_TATAMIGAESHI:
	skillratio += 10 * skill_lv;
#ifdef RENEWAL
	skillratio *= 2;
#endif
	break;
case NJ_KASUMIKIRI:
#ifdef RENEWAL
	skillratio += 20 * skill_lv;
#else
	skillratio += 10 * skill_lv;
#endif
	break;
case NJ_KIRIKAGE:
#ifdef RENEWAL
	skillratio += -50 + 150 * skill_lv;
#else
	skillratio += 100 * (skill_lv - 1);
#endif
	break;
#ifdef RENEWAL
case NJ_SYURIKEN:
	skillratio += 5 * skill_lv;
	break;
case NJ_KUNAI:
	skillratio += -100 + 100 * skill_lv;
	break;
case KN_CHARGEATK:
	skillratio += 600;
	break;
case AS_VENOMKNIFE:
	skillratio += 400;
	break;
#else
case KN_CHARGEATK:
{ // +100% every 3 cells of distance but hard-limited to 500%
	int32 k = (wd->miscflag - 1) / 3;
	if (k < 0)
		k = 0;
	else if (k > 4)
		k = 4;
	skillratio += 100 * k;
}
break;
#endif
case HT_PHANTASMIC:
#ifdef RENEWAL
	skillratio += 400;
#else
	skillratio += 50;
#endif
	break;
case MO_BALKYOUNG:
#ifdef RENEWAL
	skillratio += 700;
#else
	skillratio += 200;
#endif
	break;
case HFLI_MOON: //[orn]
	skillratio += 10 + 110 * skill_lv;
	break;
case HFLI_SBR44: //[orn]
	skillratio += 100 * (skill_lv - 1);
	break;
case NPC_VAMPIRE_GIFT:
	skillratio += ((skill_lv - 1) % 5 + 1) * 100;
	break;
case RK_SONICWAVE:
	skillratio += -100 + 1050 + 150 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case RK_HUNDREDSPEAR:
	skillratio += -100 + 600 + 200 * skill_lv;
	if (sd)
		skillratio += 50 * pc_checkskill(sd, LK_SPIRALPIERCE);
	if (sc)
	{
		if (sc->getSCE(SC_DRAGONIC_AURA))
		{
			skillratio += sc->getSCE(SC_DRAGONIC_AURA)->val1 * 160;
		}

		if (sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
			skillratio *= 2;
	}
	RE_LVL_DMOD(100);
	break;
case RK_WINDCUTTER:
	if (sd)
	{
		if (sd->weapontype1 == W_2HSWORD)
			skillratio += -100 + 250 * skill_lv;
		else if (sd->weapontype1 == W_1HSPEAR || sd->weapontype1 == W_2HSPEAR)
			skillratio += -100 + 400 * skill_lv;
		else
			skillratio += -100 + 300 * skill_lv;
	}
	else
		skillratio += -100 + 300 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case RK_IGNITIONBREAK:
	skillratio += -100 + 450 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case NPC_IGNITIONBREAK:
	// 3x3 cell Damage   = 1000  1500  2000  2500  3000 %
	// 7x7 cell Damage   = 750   1250  1750  2250  2750 %
	// 11x11 cell Damage = 500   1000  1500  2000  2500 %
	i = distance_bl(src, target);
	if (i < 2)
		skillratio += -100 + 500 * (skill_lv + 1);
	else if (i < 4)
		skillratio += -100 + 250 + 500 * skill_lv;
	else
		skillratio += -100 + 500 * skill_lv;
	break;
case RK_STORMBLAST:
	skillratio += -100 + (((sd) ? pc_checkskill(sd, RK_RUNEMASTERY) : 0) + sstatus->str / 6) * 100; // ATK = [{Rune Mastery Skill Level + (Caster's STR / 6)} x 100] %
	RE_LVL_DMOD(100);
	break;
case RK_PHANTOMTHRUST: // ATK = [{(Skill Level x 50) + (Spear Master Level x 10)} x Caster's Base Level / 150] %
	skillratio += -100 + 50 * skill_lv + 10 * (sd ? pc_checkskill(sd, KN_SPEARMASTERY) : 5);
	RE_LVL_DMOD(150); // Base level bonus.
	break;
// case NPC_PHANTOMTHRUST:	// ATK = 100% for all level
case GC_CROSSIMPACT:
	skillratio += -100 + 1400 + 150 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case GC_COUNTERSLASH:
	// ATK [{(Skill Level x 150) + 300} x Caster's Base Level / 120]% + ATK [(AGI x 2) + (Caster's Job Level x 4)]%
	skillratio += -100 + 300 + 150 * skill_lv;
	RE_LVL_DMOD(120);
	skillratio += sstatus->agi * 2;
	// If 4th job, job level of your 3rd job counts
	skillratio += (sd ? (sd->class_ & JOBL_FOURTH ? sd->change_level_4th : sd->status.job_level) * 4 : 0);
	break;
case GC_VENOMPRESSURE:
	skillratio += 900;
	break;
case GC_PHANTOMMENACE:
	skillratio += 200;
	break;
case GC_ROLLINGCUTTER:
	skillratio += -100 + 50 + 80 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case GC_CROSSRIPPERSLASHER:
	skillratio += -100 + 80 * skill_lv + (sstatus->agi * 3);
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_ROLLINGCUTTER))
		skillratio += sc->getSCE(SC_ROLLINGCUTTER)->val1 * 200;
	break;
case GC_DARKCROW:
	skillratio += 100 * (skill_lv - 1);
	break;
case AB_DUPLELIGHT_MELEE:
	skillratio += 50 + 15 * skill_lv;
	break;
case NPC_ARROWSTORM:
	if (skill_lv > 4)
		skillratio += 1900;
	else
		skillratio += 900;
	break;
case NPC_DRAGONBREATH:
	if (skill_lv > 5)
		skillratio += 500 + 500 * (skill_lv - 5); // Level 6-10 is using water element, like RK_DRAGONBREATH_WATER
	else
		skillratio += 500 + 500 * skill_lv; // Level 1-5 is using fire element, like RK_DRAGONBREATH
	break;
case RA_ARROWSTORM:
	if (sc && sc->getSCE(SC_FEARBREEZE))
		skillratio += -100 + 200 + 250 * skill_lv;
	else
		skillratio += -100 + 200 + 180 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case RA_AIMEDBOLT:
	if (sc && sc->getSCE(SC_FEARBREEZE))
		skillratio += -100 + 800 + 35 * skill_lv;
	else
		skillratio += -100 + 500 + 20 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case RA_CLUSTERBOMB:
	skillratio += 100 + 100 * skill_lv;
	break;
case RA_WUGDASH: // ATK 300%
	skillratio += 200;
	break;
case RA_WUGSTRIKE:
	skillratio += -100 + 200 * skill_lv;
	break;
case RA_WUGBITE:
	skillratio += 300 + 200 * skill_lv;
	if (skill_lv == 5)
		skillratio += 100;
	break;
case RA_SENSITIVEKEEN:
	skillratio += 50 * skill_lv;
	break;
case NC_BOOSTKNUCKLE:
	skillratio += -100 + 260 * skill_lv + sstatus->dex; // !TODO: What's the DEX bonus?
	RE_LVL_DMOD(100);
	break;
case NC_PILEBUNKER:
	skillratio += 200 + 100 * skill_lv + status_get_str(src);
	RE_LVL_DMOD(100);
	break;
case NC_VULCANARM:
	skillratio += -100 + 230 * skill_lv + sstatus->dex; // !TODO: What's the DEX bonus?
	RE_LVL_DMOD(100);
	break;
case NC_FLAMELAUNCHER:
case NC_COLDSLOWER:
	skillratio += 200 + 300 * skill_lv;
	RE_LVL_DMOD(150);
	break;
case NC_ARMSCANNON:
	skillratio += -100 + 400 + 350 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case NC_AXEBOOMERANG:
	skillratio += 150 + 50 * skill_lv;
	if (sd)
	{
		int16 index = sd->equip_index[EQI_HAND_R];

		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_WEAPON)
			skillratio += sd->inventory_data[index]->weight / 10; // Weight is divided by 10 since 10 weight in coding make 1 whole actual weight. [Rytech]
	}
	RE_LVL_DMOD(100);
	break;
case NC_POWERSWING: // According to current sources, only the str + dex gets modified by level [Akinari]
	skillratio += -100 + ((sstatus->str + sstatus->dex) / 2) + 300 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_ABR_BATTLE_WARIOR))
		skillratio *= 2;
	break;
case NC_MAGMA_ERUPTION: // 'Slam' damage
	skillratio += 350 + 50 * skill_lv;
	break;
case NC_AXETORNADO:
	skillratio += -100 + 200 + 180 * skill_lv + sstatus->vit * 2;
	if (sc && sc->getSCE(SC_AXE_STOMP))
		skillratio += 380;
	RE_LVL_DMOD(100);
	break;
case SC_FATALMENACE:
	skillratio += 120 * skill_lv + sstatus->agi; // !TODO: What's the AGI bonus?

	if (sc != nullptr && sc->getSCE(SC_ABYSS_DAGGER))
	{
		skillratio += 30 * skill_lv;
	}

	RE_LVL_DMOD(100);
	break;
case SC_TRIANGLESHOT:
	skillratio += -100 + 230 * skill_lv + 3 * sstatus->agi;
	RE_LVL_DMOD(100);
	break;
case SC_FEINTBOMB:
	skillratio += -100 + (skill_lv + 1) * sstatus->dex / 2 * ((sd) ? sd->status.job_level / 10 : 1);
	RE_LVL_DMOD(120);
	break;
case LG_CANNONSPEAR:
	skillratio += -100 + skill_lv * (120 + sstatus->str);

	if (sc != nullptr && sc->getSCE(SC_SPEAR_SCAR))
	{
		skillratio += 400;
	}

	RE_LVL_DMOD(100);
	break;
case LG_BANISHINGPOINT:
	skillratio += -100 + (100 * skill_lv);

	if (sd != nullptr)
	{
		skillratio += pc_checkskill(sd, SM_BASH) * 70;
	}

	if (sc != nullptr && sc->getSCE(SC_SPEAR_SCAR))
	{
		skillratio += 800;
	}

	RE_LVL_DMOD(100);
	break;
case LG_SHIELDPRESS:
	skillratio += -100 + 200 * skill_lv;
	if (sd)
	{
		// Shield Press only considers base STR without job bonus
		skillratio += sd->status.str;

		if (sc != nullptr && sc->getSCE(SC_SHIELD_POWER))
		{
			skillratio += skill_lv * 15 * pc_checkskill(sd, IG_SHIELD_MASTERY);
		}

		int16 index = sd->equip_index[EQI_HAND_L];

		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_ARMOR)
			skillratio += sd->inventory_data[index]->weight / 10;
	}
	RE_LVL_DMOD(100);
	break;
case LG_PINPOINTATTACK:
	skillratio += -100 + 100 * skill_lv + 5 * status_get_agi(src);
	RE_LVL_DMOD(120);
	break;
case LG_RAGEBURST:
	if (sd && sd->spiritball_old)
		skillratio += -100 + 200 * sd->spiritball_old + (status_get_max_hp(src) - status_get_hp(src)) / 100;
	else
		skillratio += 2900 + (status_get_max_hp(src) - status_get_hp(src));
	RE_LVL_DMOD(100);
	break;
case LG_MOONSLASHER:
	skillratio += -100 + 120 * skill_lv + ((sd) ? pc_checkskill(sd, LG_OVERBRAND) * 80 : 0);
	RE_LVL_DMOD(100);
	break;
case LG_OVERBRAND:
	if (sc && sc->getSCE(SC_OVERBRANDREADY))
		skillratio += -100 + 500 * skill_lv;
	else
		skillratio += -100 + 350 * skill_lv;
	skillratio += ((sd) ? pc_checkskill(sd, CR_SPEARQUICKEN) * 50 : 0);
	RE_LVL_DMOD(100);
	break;
case LG_EARTHDRIVE:
	skillratio += -100 + 380 * skill_lv + sstatus->str + sstatus->vit; // !TODO: What's the STR/VIT bonus?

	if (sc != nullptr && sc->getSCE(SC_SHIELD_POWER))
	{
		skillratio += skill_lv * 37 * pc_checkskill(sd, IG_SHIELD_MASTERY);
	}

	RE_LVL_DMOD(100);
	break;
case LG_HESPERUSLIT:
	if (sc && sc->getSCE(SC_INSPIRATION))
		skillratio += -100 + 450 * skill_lv;
	else
		skillratio += -100 + 300 * skill_lv;
	skillratio += sstatus->vit / 6; // !TODO: What's the VIT bonus?
	RE_LVL_DMOD(100);
	break;
case SR_EARTHSHAKER:
	if (tsc && ((tsc->option & (OPTION_HIDE | OPTION_CLOAK | OPTION_CHASEWALK)) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD) || tsc->getSCE(SC__SHADOWFORM)))
	{
		//[(Skill Level x 300) x (Caster Base Level / 100) + (Caster STR x 3)] %
		skillratio += -100 + 300 * skill_lv;
		RE_LVL_DMOD(100);
		skillratio += status_get_str(src) * 3;
	}
	else
	{ //[(Skill Level x 400) x (Caster Base Level / 100) + (Caster STR x 2)] %
		skillratio += -100 + 400 * skill_lv;
		RE_LVL_DMOD(100);
		skillratio += status_get_str(src) * 2;
	}
	break;

case SR_DRAGONCOMBO:
	skillratio += 100 + 80 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case SR_FALLENEMPIRE:
	// ATK [(Skill Level x 300 + 100) x Caster Base Level / 150] %
	skillratio += 300 * skill_lv;
	RE_LVL_DMOD(150);
	break;
case SR_TIGERCANNON:
{
	uint32 hp = sstatus->max_hp * (10 + (skill_lv * 2)) / 100,
		   sp = sstatus->max_sp * (5 + skill_lv) / 100;

	if (wd->miscflag & 8)
		// Base_Damage = [((Caster consumed HP + SP) / 2) x Caster Base Level / 100] %
		skillratio += -100 + (hp + sp) / 2;
	else
		// Base_Damage = [((Caster consumed HP + SP) / 4) x Caster Base Level / 100] %
		skillratio += -100 + (hp + sp) / 4;
	RE_LVL_DMOD(100);
}
	if (sc->getSCE(SC_GT_REVITALIZE))
		skillratio += skillratio * 30 / 100;
	break;
case SR_SKYNETBLOW:
	// ATK [{(Skill Level x 200) + (Caster AGI)} x Caster Base Level / 100] %
	skillratio += -100 + 200 * skill_lv + sstatus->agi / 6; // !TODO: Confirm AGI bonus
	RE_LVL_DMOD(100);
	break;

case SR_RAMPAGEBLASTER:
	if (tsc && tsc->getSCE(SC_EARTHSHAKER))
	{
		skillratio += 1400 + 550 * skill_lv;
		RE_LVL_DMOD(120);
	}
	else
	{
		skillratio += 900 + 350 * skill_lv;
		RE_LVL_DMOD(150);
	}
	if (sc->getSCE(SC_GT_CHANGE))
		skillratio += skillratio * 30 / 100;
	break;
case SR_KNUCKLEARROW:
	if (wd->miscflag & 4)
	{ // ATK [(Skill Level x 150) + (1000 x Target current weight / Maximum weight) + (Target Base Level x 5) x (Caster Base Level / 150)] %
		skillratio += -100 + 150 * skill_lv + status_get_lv(target) * 5;
		if (tsd && tsd->weight)
			skillratio += pc_getpercentweight(*tsd);
		RE_LVL_DMOD(150);
	}
	else
	{
		if (status_get_class_(target) == CLASS_BOSS)
			skillratio += 400 + 200 * skill_lv;
		else // ATK [(Skill Level x 100 + 500) x Caster Base Level / 100] %
			skillratio += 400 + 100 * skill_lv;
		RE_LVL_DMOD(100);
	}
	if (sc->getSCE(SC_GT_CHANGE))
		skillratio += skillratio * 30 / 100;
	break;
case SR_WINDMILL: // ATK [(Caster Base Level + Caster DEX) x Caster Base Level / 100] %
	skillratio += -100 + status_get_lv(src) + sstatus->dex;
	RE_LVL_DMOD(100);
	break;
case SR_GATEOFHELL:
	if (sc && sc->getSCE(SC_COMBO) && sc->getSCE(SC_COMBO)->val1 == SR_FALLENEMPIRE)
		skillratio += -100 + 800 * skill_lv;
	else
		skillratio += -100 + 500 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc->getSCE(SC_GT_REVITALIZE))
		skillratio += skillratio * 30 / 100;
	break;
case SR_GENTLETOUCH_QUIET:
	skillratio += -100 + 100 * skill_lv + sstatus->dex;
	RE_LVL_DMOD(100);
	break;
case SR_HOWLINGOFLION:
	skillratio += -100 + 500 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case SR_RIDEINLIGHTNING:
	skillratio += -100 + 40 * skill_lv;
	if (sd && sd->status.weapon == W_KNUCKLE)
		skillratio += 50 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case WM_SEVERE_RAINSTORM_MELEE:
	// ATK [{(Caster DEX / 300 + AGI / 200)} x Caster Base Level / 100] %
	skillratio += -100 + 100 * skill_lv + (sstatus->dex / 300 + sstatus->agi / 200);
	if (wd->miscflag & 4) // Whip/Instrument equipped
		skillratio += 20 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case WM_GREAT_ECHO:
	skillratio += -100 + 250 + 500 * skill_lv;
	if (sd)
	{
		skillratio += pc_checkskill(sd, WM_LESSON) * 50; // !TODO: Confirm bonus
		if (skill_check_pc_partner(sd, skill_id, &skill_lv, AREA_SIZE, 0) > 0)
			skillratio *= 2;
	}
	RE_LVL_DMOD(100);
	break;
case GN_CART_TORNADO:
{ // ATK [( Skill Level x 200 ) + ( Cart Weight / ( 150 - Caster Base STR ))] + ( Cart Remodeling Skill Level x 50 )] %
	skillratio += -100 + 200 * skill_lv;
	if (sd && sd->cart_weight)
		skillratio += sd->cart_weight / 10 / (150 - min(sd->status.str, 120)) + pc_checkskill(sd, GN_REMODELING_CART) * 50;
	if (sc && sc->getSCE(SC_BIONIC_WOODENWARRIOR))
		skillratio *= 2;
}
break;
case GN_CARTCANNON:
	skillratio += -100 + (250 + 20 * pc_checkskill(sd, GN_REMODELING_CART)) * skill_lv + 2 * sstatus->int_ / (6 - pc_checkskill(sd, GN_REMODELING_CART));
	RE_LVL_DMOD(100);
	break;
case GN_SPORE_EXPLOSION:
	skillratio += -100 + 400 + 200 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_BIONIC_WOODEN_FAIRY))
		skillratio *= 2;
	break;
case GN_WALLOFTHORN:
	skillratio += 10 * skill_lv;
	break;
case GN_CRAZYWEED_ATK:
	skillratio += -100 + 700 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case GN_SLINGITEM_RANGEMELEEATK:
	if (sd)
	{
		switch (sd->itemid)
		{
		case ITEMID_APPLE_BOMB:
			skillratio += 200 + status_get_str(src) + status_get_dex(src);
			break;
		case ITEMID_COCONUT_BOMB:
		case ITEMID_PINEAPPLE_BOMB:
			skillratio += 700 + status_get_str(src) + status_get_dex(src);
			break;
		case ITEMID_MELON_BOMB:
			skillratio += 400 + status_get_str(src) + status_get_dex(src);
			break;
		case ITEMID_BANANA_BOMB:
			skillratio += 777 + status_get_str(src) + status_get_dex(src);
			break;
		case ITEMID_BLACK_LUMP:
			skillratio += -100 + (status_get_str(src) + status_get_agi(src) + status_get_dex(src)) / 3;
			break;
		case ITEMID_BLACK_HARD_LUMP:
			skillratio += -100 + (status_get_str(src) + status_get_agi(src) + status_get_dex(src)) / 2;
			break;
		case ITEMID_VERY_HARD_LUMP:
			skillratio += -100 + status_get_str(src) + status_get_agi(src) + status_get_dex(src);
			break;
		}
		RE_LVL_DMOD(100);
	}
	break;
case GN_HELLS_PLANT_ATK:
	skillratio += -100 + 100 * skill_lv + sstatus->int_ * (sd ? pc_checkskill(sd, AM_CANNIBALIZE) : 5); // !TODO: Confirm INT and Cannibalize bonus
	RE_LVL_DMOD(100);
	break;
// Physical Elemantal Spirits Attack Skills
case EL_CIRCLE_OF_FIRE:
case EL_FIRE_BOMB_ATK:
case EL_STONE_RAIN:
	skillratio += 200;
	break;
case EL_FIRE_WAVE_ATK:
	skillratio += 500;
	break;
case EL_TIDAL_WEAPON:
	skillratio += 1400;
	break;
case EL_WIND_SLASH:
	skillratio += 100;
	break;
case EL_HURRICANE:
	skillratio += 600;
	break;
case EL_TYPOON_MIS:
case EL_WATER_SCREW_ATK:
	skillratio += 900;
	break;
case EL_STONE_HAMMER:
	skillratio += 400;
	break;
case EL_ROCK_CRUSHER:
	skillratio += 700;
	break;
case KO_JYUMONJIKIRI:
	skillratio += -100 + 200 * skill_lv;
	RE_LVL_DMOD(120);
	if (tsc && tsc->getSCE(SC_JYUMONJIKIRI))
		skillratio += skill_lv * status_get_lv(src);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		skillratio += skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
	break;
case KO_HUUMARANKA:
	skillratio += -100 + 150 * skill_lv + sstatus->str + (sd ? pc_checkskill(sd, NJ_HUUMA) * 100 : 0);
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		skillratio += skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
	break;
case KO_SETSUDAN:
	skillratio += 100 * (skill_lv - 1);
	RE_LVL_DMOD(100);
	if (tsc)
	{
		struct status_change_entry *sce;

		if ((sce = tsc->getSCE(SC_SPIRIT)) || (sce = tsc->getSCE(SC_SOULGOLEM)) || (sce = tsc->getSCE(SC_SOULSHADOW)) || (sce = tsc->getSCE(SC_SOULFALCON)) || (sce = tsc->getSCE(SC_SOULFAIRY))) // Bonus damage added when target is soul linked.
			skillratio += 200 * sce->val1;
	}
	break;
case KO_BAKURETSU:
	skillratio += -100 + (sd ? pc_checkskill(sd, NJ_TOBIDOUGU) : 1) * (50 + sstatus->dex / 4) * skill_lv * 4 / 10;
	RE_LVL_DMOD(120);
	skillratio += 10 * (sd ? sd->status.job_level : 1);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		skillratio += skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
	break;
case KO_MAKIBISHI:
	skillratio += -100 + 20 * skill_lv;
	break;
case MH_NEEDLE_OF_PARALYZE:
	skillratio += -100 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
	break;
case MH_TOXIN_OF_MANDARA:
	skillratio += -100 + 400 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
	break;
case MH_NEEDLE_STINGER:
	skillratio += -100 + 200 + 500 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
	break;
case MH_STAHL_HORN:
	skillratio += -100 + 1000 + 300 * skill_lv * status_get_lv(src) / 150 + sstatus->vit; // !TODO: Confirm VIT bonus
	break;
case MH_GLANZEN_SPIES:
	skillratio += -100 + 300 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->vit; // !TODO: Confirm VIT bonus
	break;
case MH_LAVA_SLIDE:
	skillratio += -100 + 50 * skill_lv;
	break;
case MH_BLAST_FORGE:
	skillratio += -100 + 70 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
	break;
case MH_SONIC_CRAW:
	skillratio += -100 + 60 * skill_lv * status_get_lv(src) / 150;
	break;
case MH_BLAZING_AND_FURIOUS:
	skillratio += -100 + 80 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
	break;
case MH_THE_ONE_FIGHTER_RISES:
	skillratio += -100 + 580 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
	break;
case MH_SILVERVEIN_RUSH:
	skillratio += -100 + 250 * skill_lv * status_get_lv(src) / 100 + sstatus->str; // !TODO: Confirm STR bonus
	break;
case MH_MIDNIGHT_FRENZY:
	skillratio += -100 + 450 * skill_lv * status_get_lv(src) / 150 + sstatus->str; // !TODO: Confirm STR bonus
	break;
case MH_MAGMA_FLOW:
	skillratio += -100 + (100 * skill_lv + 3 * status_get_lv(src)) * status_get_lv(src) / 120;
	break;
case RL_MASS_SPIRAL:
	skillratio += -100 + 200 * skill_lv;
	break;
case RL_FIREDANCE:
	skillratio += 100 + 100 * skill_lv;
	skillratio += (sd ? pc_checkskill(sd, GS_DESPERADO) * 20 : 0);
	RE_LVL_DMOD(100);
	break;
case RL_BANISHING_BUSTER:
	skillratio += -100 + 1000 + 200 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case RL_S_STORM:
	skillratio += -100 + 1700 + 200 * skill_lv;
	break;
case RL_SLUGSHOT:
	if (target->type == BL_MOB)
		skillratio += -100 + 1200 * skill_lv;
	else
		skillratio += -100 + 2000 * skill_lv;
	skillratio *= 2 + tstatus->size;
	break;
case RL_D_TAIL:
	skillratio += -100 + 500 + 200 * skill_lv;
	if (sd && (wd->miscflag & 8))
		skillratio *= 2;
	RE_LVL_DMOD(100);
	break;
case RL_R_TRIP:
	skillratio += -100 + 350 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case RL_R_TRIP_PLUSATK:
	skillratio += -100 + 300 + 300 * skill_lv;
	break;
case RL_H_MINE:
	if (sd && sd->flicker) // Flicker explosion damage: 500 + 300 * SkillLv
		skillratio += -100 + 500 + 300 * skill_lv;
	else // 200 + 200 * SkillLv
		skillratio += -100 + 200 + 200 * skill_lv;
	break;
case RL_HAMMER_OF_GOD:
	skillratio += -100 + 100 * skill_lv;
	if (sd)
	{
		if (wd->miscflag & 8)
			skillratio += 400 * sd->spiritball_old;
		else
			skillratio += 150 * sd->spiritball_old;
	}
	RE_LVL_DMOD(100);
	break;
case RL_FIRE_RAIN:
case RL_AM_BLAST:
	skillratio += -100 + 3500 + 300 * skill_lv;
	break;
case SU_BITE:
	skillratio += 100;
	break;
case SU_SCRATCH:
	skillratio += -50 + 50 * skill_lv;
	break;
case SU_SCAROFTAROU:
	skillratio += -100 + 100 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		skillratio += skillratio * status_get_hp(src) / status_get_max_hp(src);
	break;
case SU_PICKYPECK:
case SU_PICKYPECK_DOUBLE_ATK:
	skillratio += 100 + 100 * skill_lv;
	if (status_get_hp(target) < (status_get_max_hp(target) / 2))
		skillratio *= 2;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		skillratio += skillratio * status_get_hp(src) / status_get_max_hp(src);
	break;
case SU_LUNATICCARROTBEAT:
case SU_LUNATICCARROTBEAT2:
	skillratio += 100 + 100 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		skillratio += skillratio * status_get_hp(src) / status_get_max_hp(src);
	if (status_get_lv(src) > 99)
		skillratio += sstatus->str;
	RE_LVL_DMOD(100);
	break;
case SU_SVG_SPIRIT:
	skillratio += 150 + 150 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		skillratio += skillratio * status_get_hp(src) / status_get_max_hp(src);
	break;
case SJ_FULLMOONKICK:
	skillratio += 1000 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFMOON))
		skillratio += skillratio * sc->getSCE(SC_LIGHTOFMOON)->val2 / 100;
	break;
case SJ_NEWMOONKICK:
	skillratio += 600 + 100 * skill_lv;
	break;
case SJ_STAREMPEROR:
	skillratio += 700 + 200 * skill_lv;
	break;
case SJ_SOLARBURST:
	skillratio += 900 + 220 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFSUN))
		skillratio += skillratio * sc->getSCE(SC_LIGHTOFSUN)->val2 / 100;
	break;
case SJ_PROMINENCEKICK:
	skillratio += 50 + 50 * skill_lv;
	break;
case SJ_FALLINGSTAR_ATK:
case SJ_FALLINGSTAR_ATK2:
	skillratio += 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFSTAR))
		skillratio += skillratio * sc->getSCE(SC_LIGHTOFSTAR)->val2 / 100;
	break;
case DK_SERVANTWEAPON_ATK:
	skillratio += -100 + 600 + 850 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case DK_SERVANT_W_PHANTOM:
	skillratio += -100 + 200 + 300 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case DK_SERVANT_W_DEMOL:
	skillratio += -100 + 500 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case DK_HACKANDSLASHER:
case DK_HACKANDSLASHER_ATK:
	skillratio += -100 + 350 + 820 * skill_lv;
	skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case DK_DRAGONIC_AURA:
	skillratio += 3650 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_ANGEL)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case DK_MADNESS_CRUSHER:
	skillratio += -100 + 1000 + 3800 * skill_lv;
	skillratio += 10 * sstatus->pow;
	if (sd != nullptr)
	{
		int16 index = sd->equip_index[EQI_HAND_R];

		if (index >= 0 && sd->inventory_data[index] != nullptr)
		{
			skillratio += sd->inventory_data[index]->weight / 10 * sd->inventory_data[index]->weapon_level;
		}
	}
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		skillratio *= 2;
	break;
case DK_STORMSLASH:
	skillratio += -100 + 300 + 750 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_GIANTGROWTH) && rnd_chance(60, 100))
		skillratio *= 2;
	break;
case DK_DRAGONIC_BREATH:
	skillratio += -100 + 50 + 350 * skill_lv;
	skillratio += 7 * sstatus->pow;

	if (sc && sc->getSCE(SC_DRAGONIC_AURA))
	{
		skillratio += 3 * sstatus->pow;
		skillratio += (skill_lv * (sstatus->max_hp * 25 / 100) * 7) / 100; // Skill level x 0.07 x ((MaxHP / 4) + (MaxSP / 2))
		skillratio += (skill_lv * (sstatus->max_sp * 50 / 100) * 7) / 100;
	}
	else
	{
		skillratio += (skill_lv * (sstatus->max_hp * 25 / 100) * 5) / 100; // Skill level x 0.05 x ((MaxHP / 4) + (MaxSP / 2))
		skillratio += (skill_lv * (sstatus->max_sp * 50 / 100) * 5) / 100;
	}

	RE_LVL_DMOD(100);
	break;
case DK_DRAGONIC_PIERCE:
	skillratio += -100 + 850 + 600 * skill_lv;
	skillratio += 7 * sstatus->pow; // !TODO: unknown ratio

	if (sc != nullptr && sc->hasSCE(SC_DRAGONIC_AURA))
		skillratio += 100 + 50 * skill_lv;

	RE_LVL_DMOD(100);
	break;
case IQ_OLEUM_SANCTUM:
	skillratio += -100 + 500 + 2000 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case IQ_MASSIVE_F_BLASTER:
	skillratio += -100 + 2300 * skill_lv + 15 * sstatus->pow;
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_DEMON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case IQ_EXPOSION_BLASTER:
	skillratio += -100 + 450 + 2600 * skill_lv;
	skillratio += 10 * sstatus->pow;

	if (tsc != nullptr && tsc->getSCE(SC_HOLY_OIL))
	{
		skillratio += 950 * skill_lv;
	}

	RE_LVL_DMOD(100);
	break;
case IQ_FIRST_BRAND:
	skillratio += -100 + 1200 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case IQ_SECOND_FLAME:
	skillratio += -100 + 200 + 2900 * skill_lv + 9 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case IQ_SECOND_FAITH:
	skillratio += -100 + 100 + 2300 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case IQ_SECOND_JUDGEMENT:
	skillratio += -100 + 150 + 2600 * skill_lv + 7 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case IQ_THIRD_PUNISH:
	skillratio += -100 + 450 + 1800 * skill_lv;
	skillratio += 10 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case IQ_THIRD_FLAME_BOMB:
	skillratio += -100 + 650 * skill_lv + 10 * sstatus->pow;
	skillratio += sstatus->max_hp * 20 / 100;
	RE_LVL_DMOD(100);
	break;
case IQ_THIRD_CONSECRATION:
	skillratio += -100 + 700 * skill_lv + 10 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case IG_GRAND_JUDGEMENT:
	skillratio += -100 + 250 + 1500 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_PLANT || tstatus->race == RC_INSECT)
		skillratio += 100 + 150 * skill_lv;
	RE_LVL_DMOD(100);
	if ((i = pc_checkskill_imperial_guard(sd, 3)) > 0)
		skillratio += skillratio * i / 100;
	break;
case IG_SHIELD_SHOOTING:
	skillratio += -100 + 1000 + 3500 * skill_lv;
	skillratio += 10 * sstatus->pow;
	skillratio += skill_lv * 150 * pc_checkskill(sd, IG_SHIELD_MASTERY);
	if (sd)
	{ // Damage affected by the shield's weight and refine. Need official formula. [Rytech]
		int16 index = sd->equip_index[EQI_HAND_L];

		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_ARMOR)
		{
			skillratio += (sd->inventory_data[index]->weight * 7 / 6) / 10;
			skillratio += sd->inventory.u.items_inventory[index].refine * 100;
		}
	}
	RE_LVL_DMOD(100);
	break;
case IG_OVERSLASH:
	skillratio += -100 + 220 * skill_lv;
	skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 50 * skill_lv;
	skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
	if ((i = pc_checkskill_imperial_guard(sd, 3)) > 0)
		skillratio += skillratio * i / 100;
	break;
case IG_RADIANT_SPEAR:
	skillratio += -100 + 3500 + 1150 * skill_lv;
	skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 50;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio

	if (sc != nullptr && sc->getSCE(SC_SPEAR_SCAR))
		skillratio += 250 * skill_lv;

	RE_LVL_DMOD(100);
	break;
case IG_IMPERIAL_CROSS:
	skillratio += -100 + 1650 + 1350 * skill_lv;
	skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 25;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio

	if (sc != nullptr && sc->getSCE(SC_SPEAR_SCAR))
		skillratio += 100 + 300 * skill_lv;

	RE_LVL_DMOD(100);
	break;
case CD_EFFLIGO:
	skillratio += -100 + 1650 * skill_lv + 7 * sstatus->pow;
	skillratio += 8 * pc_checkskill(sd, CD_MACE_BOOK_M);
	if (tstatus->race == RC_UNDEAD || tstatus->race == RC_DEMON)
	{
		skillratio += 150 * skill_lv;
		skillratio += 7 * pc_checkskill(sd, CD_MACE_BOOK_M);
	}
	RE_LVL_DMOD(100);
	break;
case CD_PETITIO:
	skillratio += -100 + (1050 + pc_checkskill(sd, CD_MACE_BOOK_M) * 50) * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SHC_DANCING_KNIFE:
	skillratio += -100 + 200 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SHC_SAVAGE_IMPACT:
	skillratio += -100 + 105 * skill_lv + 5 * sstatus->pow;

	if (sc != nullptr && sc->getSCE(SC_SHADOW_EXCEED))
	{
		skillratio += 20 * skill_lv + 3 * sstatus->pow; // !TODO: check POW ratio
	}

	RE_LVL_DMOD(100);
	break;
case SHC_ETERNAL_SLASH:
	skillratio += -100 + 300 * skill_lv + 2 * sstatus->pow;

	if (sc != nullptr && sc->getSCE(SC_SHADOW_EXCEED))
	{
		skillratio += 120 * skill_lv + sstatus->pow;
	}

	RE_LVL_DMOD(100);
	break;
case SHC_SHADOW_STAB:
	skillratio += -100 + 550 * skill_lv;
	skillratio += 5 * sstatus->pow;

	if (wd->miscflag & SKILL_ALTDMG_FLAG)
	{
		skillratio += 100 * skill_lv + 2 * sstatus->pow;
	}

	RE_LVL_DMOD(100);
	break;
case SHC_IMPACT_CRATER:
	skillratio += -100 + 80 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SHC_FATAL_SHADOW_CROW:
	skillratio += -100 + 1300 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_DRAGON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case SHC_CROSS_SLASH:
	skillratio += -100 + 300 * skill_lv;
	skillratio += 5 * sstatus->pow;

	if (sc != nullptr && sc->getSCE(SC_SHADOW_EXCEED))
	{
		skillratio += 60 * skill_lv;
		skillratio += 2 * sstatus->pow;
	}
	RE_LVL_DMOD(100);
	break;
case MT_AXE_STOMP:
	skillratio += -100 + 450 + 1150 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case MT_MIGHTY_SMASH:
	skillratio += -100 + 80 + 240 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (sc && sc->getSCE(SC_AXE_STOMP))
	{
		skillratio += 20;
		skillratio += 5 * sstatus->pow;
	}
	RE_LVL_DMOD(100);
	break;
case MT_RUSH_QUAKE:
	skillratio += -100 + 3600 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_FORMLESS || tstatus->race == RC_INSECT)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case MT_A_MACHINE: // Formula unknown. Using Dancing Knife's formula for now. [Rytech]
	skillratio += -100 + 200 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case MT_SPARK_BLASTER:
	skillratio += -100 + 600 + 1400 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case MT_TRIPLE_LASER:
	skillratio += -100 + 650 + 1150 * skill_lv;
	skillratio += 12 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case MT_RUSH_STRIKE:
	skillratio += -100 + 3500 * skill_lv;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	RE_LVL_DMOD(100);
	break;
case MT_POWERFUL_SWING:
	skillratio += -100 + 300 + 850 * skill_lv;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	if (sc && sc->getSCE(SC_AXE_STOMP))
		skillratio += 100 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case MT_ENERGY_CANNONADE:
	skillratio += -100 + 250 + 750 * skill_lv;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	RE_LVL_DMOD(100);
	break;
case ABC_ABYSS_DAGGER:
	skillratio += -100 + 350 + 1400 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case ABC_UNLUCKY_RUSH:
	skillratio += -100 + 100 + 300 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case ABC_CHAIN_REACTION_SHOT:
	skillratio += -100 + 850 * skill_lv;
	skillratio += 15 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case ABC_CHAIN_REACTION_SHOT_ATK:
	skillratio += -100 + 800 + 2550 * skill_lv;
	skillratio += 15 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case ABC_DEFT_STAB:
	skillratio += -100 + 700 + 550 * skill_lv;
	skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case ABC_FRENZY_SHOT:
	skillratio += -100 + 250 + 800 * skill_lv;
	skillratio += 15 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case WH_HAWKRUSH:
	skillratio += -100 + 500 * skill_lv + 5 * sstatus->con;
	if (sd)
		skillratio += skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	RE_LVL_DMOD(100);
	break;
case WH_HAWKBOOMERANG:
	skillratio += -100 + 600 * skill_lv + 10 * sstatus->con;
	if (sd)
		skillratio += skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH)
		skillratio += skillratio * 50 / 100;
	RE_LVL_DMOD(100);
	break;
case WH_GALESTORM:
	skillratio += -100 + 1350 * skill_lv;
	skillratio += 10 * sstatus->con;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CALAMITYGALE) && (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH))
		skillratio += skillratio * 50 / 100;
	break;
case WH_CRESCIVE_BOLT:
	skillratio += -100 + 500 + 1300 * skill_lv;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	if (sc)
	{
		if (sc->getSCE(SC_CRESCIVEBOLT))
			skillratio += skillratio * (20 * sc->getSCE(SC_CRESCIVEBOLT)->val1) / 100;

		if (sc->getSCE(SC_CALAMITYGALE))
		{
			skillratio += skillratio * 20 / 100;

			if (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH)
				skillratio += skillratio * 50 / 100;
		}
	}
	break;
case WH_DEEPBLINDTRAP:
case WH_SOLIDTRAP:
case WH_SWIFTTRAP:
case WH_FLAMETRAP:
	skillratio += -100 + 850 * skill_lv + 5 * sstatus->con;
	RE_LVL_DMOD(100);
	skillratio += skillratio * (20 * (sd ? pc_checkskill(sd, WH_ADVANCED_TRAP) : 5)) / 100;
	break;
case WH_WILD_WALK:
	skillratio += -100 + 1800 + 2800 * skill_lv;
	// !TODO: unknown con and WH_NATUREFRIENDLY/HT_STEELCROW skills ratio
	skillratio += 5 * sstatus->con;
	skillratio += skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	skillratio += skillratio * pc_checkskill(sd, HT_STEELCROW) / 10;
	RE_LVL_DMOD(100);
	break;
case BO_ACIDIFIED_ZONE_WATER:
case BO_ACIDIFIED_ZONE_GROUND:
case BO_ACIDIFIED_ZONE_WIND:
case BO_ACIDIFIED_ZONE_FIRE:
case BO_ACIDIFIED_ZONE_WATER_ATK: // These deal the same damage? [Rytech]
case BO_ACIDIFIED_ZONE_GROUND_ATK:
case BO_ACIDIFIED_ZONE_WIND_ATK:
case BO_ACIDIFIED_ZONE_FIRE_ATK:
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if (sc != nullptr && sc->getSCE(SC_RESEARCHREPORT))
	{
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
	break;
case BO_EXPLOSIVE_POWDER:
	skillratio += -100 + 500 + 650 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (sc && sc->getSCE(SC_RESEARCHREPORT))
		skillratio += 100 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case BO_MAYHEMIC_THORNS:
	skillratio += -100 + 200 + 300 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (sc && sc->getSCE(SC_RESEARCHREPORT))
		skillratio += 150;
	RE_LVL_DMOD(100);
	break;
case BO_MYSTERY_POWDER:
	skillratio += -100 + 1500 + 4000 * skill_lv;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	RE_LVL_DMOD(100);
	break;
case BO_DUST_EXPLOSION:
	skillratio += -100 + 450 + 600 * skill_lv;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	if (sc && sc->getSCE(SC_RESEARCHREPORT))
		skillratio += 200 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case TR_ROSEBLOSSOM:
	skillratio += -100 + 200 + 2000 * skill_lv;

	if (sd && pc_checkskill(sd, TR_STAGE_MANNER) > 0)
		skillratio += 3 * sstatus->con;

	if (tsc != nullptr && tsc->getSCE(SC_SOUNDBLEND))
	{
		skillratio += 200 * skill_lv;
	}

	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY))
	{
		skillratio *= 2;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			skillratio += skillratio * 50 / 100;
	}
	break;
case TR_ROSEBLOSSOM_ATK:
	skillratio += -100 + 250 + 2800 * skill_lv;

	if (sd && pc_checkskill(sd, TR_STAGE_MANNER) > 0)
		skillratio += 3 * sstatus->con;

	if (tsc != nullptr && tsc->getSCE(SC_SOUNDBLEND))
	{
		skillratio += 200 * skill_lv;
	}

	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY))
	{
		skillratio *= 2;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			skillratio += skillratio * 50 / 100;
	}
	break;
case TR_RHYTHMSHOOTING:
	skillratio += -100 + 550 + 950 * skill_lv;

	if (sd && pc_checkskill(sd, TR_STAGE_MANNER) > 0)
		skillratio += 5 * sstatus->con;

	if (tsc && tsc->getSCE(SC_SOUNDBLEND))
	{
		skillratio += 300 + 100 * skill_lv;
		skillratio += 2 * sstatus->con;
	}

	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY))
	{
		skillratio *= 2;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			skillratio += skillratio * 50 / 100;
	}
	break;
case ABR_BATTLE_BUSTER:	   // Need official formula.
case ABR_DUAL_CANNON_FIRE: // Need official formula.
	skillratio += -100 + 8000;
	break;
case ABR_INFINITY_BUSTER: // Need official formula.
	skillratio += -100 + 50000;
	break;
case HN_SPIRAL_PIERCE_MAX:
	skillratio += -100 + 1000 + 1500 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;
	switch (status_get_size(target))
	{
	case SZ_SMALL:
		skillratio = skillratio * 150 / 100;
		break;
	case SZ_MEDIUM:
		skillratio = skillratio * 130 / 100;
		break;
	case SZ_BIG:
		skillratio = skillratio * 120 / 100;
		break;
	}
	RE_LVL_DMOD(100);
	break;
case HN_SHIELD_CHAIN_RUSH:
	skillratio += -100 + 850 + 1050 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case HN_MEGA_SONIC_BLOW:
	skillratio += -100 + 900 + 750 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (status_get_hp(target) < status_get_max_hp(target) / 2)
		skillratio *= 2;
	RE_LVL_DMOD(100);
	break;
case HN_DOUBLEBOWLINGBASH:
	skillratio += -100 + 250 + 400 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case NW_HASTY_FIRE_IN_THE_HOLE:
	skillratio += -100 + 1500 + 1500 * skill_lv;
	skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 20;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case NW_BASIC_GRENADE:
	skillratio += -100 + 1500 + 2100 * skill_lv;
	skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 50;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case NW_GRENADES_DROPPING:
	skillratio += -100 + 550 + 850 * skill_lv;
	skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 30;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case NW_WILD_FIRE:
	skillratio += -100 + 1500 + 3000 * skill_lv;
	skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 500 * skill_lv;
	if (sd && sd->weapontype1 == W_SHOTGUN)
		skillratio += 200 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case NW_MAGAZINE_FOR_ONE:
	skillratio += -100 + 250 + 500 * skill_lv;
	skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 100 * skill_lv;
	if (sd && sd->weapontype1 == W_REVOLVER)
		skillratio += 50 + 300 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case NW_SPIRAL_SHOOTING:
	skillratio += -100 + 1200 + 1700 * skill_lv;
	skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 150 * skill_lv;
	if (sd && sd->weapontype1 == W_RIFLE)
		skillratio += 200 + 1100 * skill_lv;
	RE_LVL_DMOD(100);
	break;
case NW_ONLY_ONE_BULLET:
	skillratio += -100 + 1200 + 3000 * skill_lv;
	skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 350 * skill_lv;
	if (sd && sd->weapontype1 == W_REVOLVER)
	{
		skillratio += 400 * skill_lv;
	}
	RE_LVL_DMOD(100);
	break;
case NW_THE_VIGILANTE_AT_NIGHT:
	if (sd && sd->weapontype1 == W_GATLING)
	{
		skillratio += -100 + 300 * skill_lv;
		if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
			skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 100 * skill_lv;
	}
	else
	{
		skillratio += -100 + 800 + 700 * skill_lv;
		if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
			skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 200 * skill_lv;
	}
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case NW_MISSION_BOMBARD:
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
	{
		skillratio += -100 + 5000 + 1800 * skill_lv;
		skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 100;
	}
	else
	{
		skillratio += -100 + 800 + 200 * skill_lv;
		skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 30;
	}
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case SH_CHUL_HO_SONIC_CLAW:
	skillratio += -100 + 1100 + 2200 * skill_lv;
	skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->pow;

	if (pc_checkskill(sd, SH_COMMUNE_WITH_CHUL_HO) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
	{
		skillratio += 400 * skill_lv;
		skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	}
	RE_LVL_DMOD(100);
	break;
case SH_HOWLING_OF_CHUL_HO:
	skillratio += -100 + 600 + 1050 * skill_lv;
	skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->pow;

	if (pc_checkskill(sd, SH_COMMUNE_WITH_CHUL_HO) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
	{
		skillratio += 100 + 100 * skill_lv;
		skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	}
	RE_LVL_DMOD(100);
	break;
case SH_HOGOGONG_STRIKE:
	skillratio += -100 + 180 + 200 * skill_lv;
	skillratio += 10 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->pow;

	if (pc_checkskill(sd, SH_COMMUNE_WITH_CHUL_HO) > 0 || (sc != nullptr && sc->getSCE(SC_TEMPORARY_COMMUNION) != nullptr))
	{
		skillratio += 70 + 150 * skill_lv;
		skillratio += 10 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	}
	RE_LVL_DMOD(100);
	break;
case SH_CHUL_HO_BATTERING:
	skillratio += -100 + 480 + 160 * skill_lv;
	skillratio += 70 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SKE_MIDNIGHT_KICK:
	skillratio += -100 + 800 + 1500 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;

	if (sc != nullptr && (sc->getSCE(SC_MIDNIGHT_MOON) != nullptr || sc->getSCE(SC_SKY_ENCHANT) != nullptr))
	{
		skillratio += 950 + 250 * skill_lv;
	}

	RE_LVL_DMOD(100);
	break;

case SKE_ALL_IN_THE_SKY:
	skillratio += -100 + 250 + 1200 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (status_get_race(target) == RC_DEMIHUMAN || status_get_race(target) == RC_DEMON)
		wd->div_ = 3;
	break;

case SKE_TWINKLING_GALAXY:
	skillratio += -100 + 300 + 500 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;

case SKE_STAR_CANNON:
	skillratio += -100 + 250 + 550 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;

case SKE_STAR_BURST:
	skillratio += -100 + 500 + 400 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;

case SKE_DAWN_BREAK:
	skillratio += -100 + 600 + 700 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;

	if (sc != nullptr && (sc->getSCE(SC_DAWN_MOON) != nullptr || sc->getSCE(SC_SKY_ENCHANT) != nullptr))
	{
		skillratio += 200 * skill_lv;
	}

	RE_LVL_DMOD(100);
	break;

case SKE_SUNSET_BLAST:
	skillratio += -100 + 950 + 400 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;

case SKE_RISING_MOON:
	skillratio += -100 + 700 + 450 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;

case SKE_NOON_BLAST:
	skillratio += -100 + 1500 + 1250 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;

case SKE_RISING_SUN:
	skillratio += -100 + 500 + 600 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SS_SHIMIRU:
	skillratio += -100 + 700 * skill_lv;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	break;
case SS_KAGEGARI:
	skillratio += -100 + 500 + 400 * skill_lv;
	skillratio += pc_checkskill(sd, SS_KAGEGISSEN) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SS_KAGEAKUMU:
case SS_HITOUAKUMU:
	skillratio += -100 + 18000 + 5 * sstatus->pow;

	if (tsc != nullptr && tsc->getSCE(SC_NIGHTMARE) != nullptr)
	{
		skillratio += skillratio / 2;
	}

	RE_LVL_DMOD(100);
	break;
case SS_KAGENOMAI:
	skillratio += -100 + 400 + 550 * skill_lv;
	skillratio += pc_checkskill(sd, SS_KAGEGARI) * 50 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
		skillratio = skillratio * 3 / 10;
	break;
case SS_FUUMASHOUAKU:
	skillratio += -100 + 700 + 200 * skill_lv;
	skillratio += pc_checkskill(sd, SS_FUUMAKOUCHIKU) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SS_FUUMAKOUCHIKU:
	skillratio += -100 + 600 + 400 * skill_lv;
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
	{
		skillratio += 200;
	}
	skillratio += pc_checkskill(sd, SS_FUUMASHOUAKU) * 30 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SS_KUNAIWAIKYOKU:
	skillratio += -100 + 300 + 600 * skill_lv;
	skillratio += pc_checkskill(sd, SS_KUNAIKUSSETSU) * 10 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
		skillratio = skillratio * 3 / 10;
	break;
case SS_KUNAIKUSSETSU:
	skillratio += -100 + 200 + 360 * skill_lv;
	skillratio += pc_checkskill(sd, SS_KUNAIKAITEN) * 10 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SS_KUNAIKAITEN:
	skillratio += -100 + 800 + 700 * skill_lv;
	skillratio += pc_checkskill(sd, SS_KUNAIWAIKYOKU) * 70 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	break;
case SS_KAGEGISSEN:
	skillratio += -100 + 1600 + 700 * skill_lv;
	skillratio += pc_checkskill(sd, SS_KAGENOMAI) * 100 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
		skillratio = skillratio * 3 / 10;
	break;
}

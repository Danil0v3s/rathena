switch (skill_id)
{ // Hit skill modifiers
case MS_MAGNUM:
case SM_MAGNUM:
	hitrate += hitrate * 10 * skill_lv / 100;
	break;
case KN_AUTOCOUNTER:
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
case NPC_POISON:
case NPC_BLINDATTACK:
case NPC_SILENCEATTACK:
case NPC_STUNATTACK:
case NPC_PETRIFYATTACK:
case NPC_CURSEATTACK:
case NPC_SLEEPATTACK:
case NPC_BLEEDING:
case NPC_BLEEDING2:
	hitrate += hitrate * 20 / 100;
	break;
case NPC_FIREBREATH:
case NPC_ICEBREATH:
case NPC_ICEBREATH2:
case NPC_THUNDERBREATH:
case NPC_ACIDBREATH:
case NPC_DARKNESSBREATH:
	hitrate *= 2;
	break;
case KN_PIERCE:
case ML_PIERCE:
	hitrate += hitrate * 5 * skill_lv / 100;
	break;
case AS_SONICBLOW:
	if (sd && pc_checkskill(sd, AS_SONICACCEL) > 0)
#ifdef RENEWAL
		hitrate += hitrate * 90 / 100;
#else
		hitrate += hitrate * 50 / 100;
#endif
	break;
#ifdef RENEWAL
case RG_BACKSTAP:
	hitrate += skill_lv; // !TODO: What's the rate increase?
	break;
#endif
case RK_SONICWAVE:
	hitrate += hitrate * 3 * skill_lv / 100; // !TODO: Confirm the hitrate bonus
	break;
case MC_CARTREVOLUTION:
case GN_CART_TORNADO:
case GN_CARTCANNON:
	if (sd && pc_checkskill(sd, GN_REMODELING_CART))
		hitrate += pc_checkskill(sd, GN_REMODELING_CART) * 4;
	break;
case LG_BANISHINGPOINT:
	hitrate += 5 * skill_lv;
	break;
case GC_VENOMPRESSURE:
	hitrate += 10 + 4 * skill_lv;
	break;
case SC_FATALMENACE:
	if (skill_lv < 6)
		hitrate -= 35 - 5 * skill_lv;
	else if (skill_lv > 6)
		hitrate += 5 * skill_lv - 30;
	break;
case RL_SLUGSHOT:
{
	int8 dist = distance_bl(src, target);
	if (dist > 3)
	{
		// Reduce n hitrate for each cell after initial 3 cells. Different each level
		// -10:-9:-8:-7:-6
		dist -= 3;
		hitrate -= ((11 - skill_lv) * dist);
	}
}
break;
}
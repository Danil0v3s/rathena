// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "skill_factory_swordsman.hpp"

#include "autoberserk.hpp"
#include "bash.hpp"
#include "endure.hpp"
#include "fatalblow.hpp"
#include "magnum.hpp"
#include "movingrecovery.hpp"
#include "provoke.hpp"
#include "recovery.hpp"
#include "selfprovoke.hpp"
#include "sword.hpp"
#include "twohand.hpp"

std::unique_ptr<const SkillImpl> SkillFactorySwordsman::create(const e_skill skill_id) const
{
	switch (skill_id)
	{
	case SM_AUTOBERSERK:
		return std::make_unique<SkillAutoBerserk>();
	case SM_BASH:
		return std::make_unique<SkillBash>();
	case SM_ENDURE:
		return std::make_unique<SkillAutoBerserk>();
	case SM_FATALBLOW:
		return std::make_unique<SkillFatalBlow>();
	case SM_MAGNUM:
		return std::make_unique<SkillMagnumBreak>();
	case SM_MOVINGRECOVERY:
		return std::make_unique<SkillMovingHpRecovery>();
	case SM_PROVOKE:
		return std::make_unique<SkillProvoke>();
	case SM_RECOVERY:
		return std::make_unique<SkillIncreaseHpRecovery>();
	case SM_SELFPROVOKE:
		return std::make_unique<SkillProvokeSelf>();
	case SM_SWORD:
		return std::make_unique<SkillSwordMastery>();
	case SM_TWOHAND:
		return std::make_unique<SkillTwoHandSwordMastery>();

	default:
		return nullptr;
	}
}

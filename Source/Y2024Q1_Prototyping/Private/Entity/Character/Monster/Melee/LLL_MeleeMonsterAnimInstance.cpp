// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/LLL_MeleeMonsterAnimInstance.h"

#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"
#include "Util/LLLConstructorHelper.h"

ULLL_MeleeMonsterAnimInstance::ULLL_MeleeMonsterAnimInstance()
{
	MeleeMonsterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MeleeMonsterDataAsset>(PATH_MELEE_MONSTER_DATA, EAssertionLevel::Check);
}

void ULLL_MeleeMonsterAnimInstance::PlayAttackAnimation()
{
	Montage_Play(MeleeMonsterDataAsset->AttackAnimMontage);
}

void ULLL_MeleeMonsterAnimInstance::AnimNotify_Hit()
{
	ALLL_MeleeMonster* MeleeMonster = Cast<ALLL_MeleeMonster>(TryGetPawnOwner());
	if (IsValid(MeleeMonster))
	{
		MeleeMonster->DamageToPlayer();
	}
}

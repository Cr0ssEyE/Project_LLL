// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonsterAnimInstance.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"
#include "Util/LLLConstructorHelper.h"

ULLL_RangedMonsterAnimInstance::ULLL_RangedMonsterAnimInstance()
{
	MonsterBaseDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_RangedMonsterDataAsset>(PATH_RANGED_MONSTER_DATA, EAssertionLevel::Check);
}

void ULLL_RangedMonsterAnimInstance::PlayAttackAnimation()
{
	Montage_Play(MonsterBaseDataAsset->AttackAnimMontage);
}

void ULLL_RangedMonsterAnimInstance::AnimNotify_Release()
{
	ALLL_RangedMonster* RangedMonster = Cast<ALLL_RangedMonster>(TryGetPawnOwner());
	if (IsValid(RangedMonster))
	{
		RangedMonster->ThrowToPlayer();
	}
}

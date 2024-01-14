// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Util/LLLConstructorHelper.h"

ULLL_MonsterBaseAnimInstance::ULLL_MonsterBaseAnimInstance()
{
	MonsterBaseDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MonsterBaseDataAsset>(PATH_MONSTER_DATA, EAssertionLevel::Check);
}

void ULLL_MonsterBaseAnimInstance::PlayAttackAnimation()
{
	Montage_Play(MonsterBaseDataAsset->AttackAnimMontage);
}

void ULLL_MonsterBaseAnimInstance::AnimNotify_Hit()
{
	ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(TryGetPawnOwner());
	if (IsValid(MonsterBase))
	{
		MonsterBase->DamageToPlayer();
	}
}

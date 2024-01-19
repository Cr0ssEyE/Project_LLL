// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonsterAnimInstance.h"

#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"

void ULLL_RangedMonsterAnimInstance::AnimNotify_Release()
{
	ALLL_RangedMonster* RangedMonster = Cast<ALLL_RangedMonster>(TryGetPawnOwner());
	if (IsValid(RangedMonster))
	{
		RangedMonster->ThrowToPlayer();
	}
}

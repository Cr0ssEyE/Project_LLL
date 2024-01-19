// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/LLL_MeleeMonsterAnimInstance.h"

#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"

void ULLL_MeleeMonsterAnimInstance::AnimNotify_Hit()
{
	ALLL_MeleeMonster* MeleeMonster = Cast<ALLL_MeleeMonster>(TryGetPawnOwner());
	if (IsValid(MeleeMonster))
	{
		MeleeMonster->DamageToPlayer();
	}
}

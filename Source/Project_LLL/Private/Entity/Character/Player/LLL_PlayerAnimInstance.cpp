// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"

#include "Entity/Character/Base/LLL_BaseCharacter.h"

void ULLL_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerDataAsset = Cast<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);
}

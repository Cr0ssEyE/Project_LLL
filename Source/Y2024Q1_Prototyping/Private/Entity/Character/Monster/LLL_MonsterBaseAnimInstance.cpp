// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"

void ULLL_MonsterBaseAnimInstance::SetDataAsset(const ULLL_BaseCharacterDataAsset* InCharacterDataAsset)
{
	Super::SetDataAsset(InCharacterDataAsset);
	
	MonsterBaseDataAsset = Cast<ULLL_MonsterBaseDataAsset>(CharacterDataAsset);
}

void ULLL_MonsterBaseAnimInstance::PlayAttackAnimation()
{
	Montage_Play(CharacterDataAsset->AttackAnimMontage);
}

void ULLL_MonsterBaseAnimInstance::PlayDamagedAnimation()
{
	Montage_Play(MonsterBaseDataAsset->DamagedAnimMontage);
}

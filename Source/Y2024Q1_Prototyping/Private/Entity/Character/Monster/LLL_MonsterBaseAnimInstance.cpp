// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"

void ULLL_MonsterBaseAnimInstance::PlayAttackAnimation()
{
	Montage_Play(CharacterDataAsset->AttackAnimMontage);
}

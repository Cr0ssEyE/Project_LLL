// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/LLL_MonsterBaseAnimInstance.h"

#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"

ULLL_MonsterBaseAnimInstance::ULLL_MonsterBaseAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<ULLL_MonsterBaseDataAsset> DA_Monster(TEXT("/Script/Y2024Q1_Prototyping.LLL_MonsterBaseDataAsset'/Game/DataAsset/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}

	bAttackAnimationIsPlaying = false;
}

void ULLL_MonsterBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OnMontageEnded.AddDynamic(this, &ULLL_MonsterBaseAnimInstance::MontageEnd);
}

void ULLL_MonsterBaseAnimInstance::PlayAttackAnimation()
{
	if (bAttackAnimationIsPlaying)
	{
		return;
	}

	Montage_Play(MonsterDataAsset->AttackAnimMontage);
	
	bAttackAnimationIsPlaying = true;
}

void ULLL_MonsterBaseAnimInstance::AnimNotify_Hit()
{
	if (ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(TryGetPawnOwner()))
	{
		MonsterBase->DamageToPlayer();
	}
}

void ULLL_MonsterBaseAnimInstance::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MonsterDataAsset->AttackAnimMontage)
	{
		bAttackAnimationIsPlaying = false;
		AttackAnimationEndDelegate.Broadcast();
	}
}

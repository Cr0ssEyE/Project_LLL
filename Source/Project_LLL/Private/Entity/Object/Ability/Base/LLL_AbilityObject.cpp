// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"

#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/LLL_AbilityObjectDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Attribute/Object/Ability/Base/LLL_AbilityObjectAttributeSet.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "Util/LLL_AbilityDataHelper.h"
#include "Util/LLL_MathHelper.h"

ALLL_AbilityObject::ALLL_AbilityObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	OverlapCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Sphere Collision"));
	OverlapCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapCollisionSphere->SetCollisionProfileName(CP_PLAYER_ABILITY_OBJECT);
	OverlapCollisionSphere->SetupAttachment(RootComponent);
}

void ALLL_AbilityObject::BeginPlay()
{
	Super::BeginPlay();

	AbilityObjectDataAsset = Cast<ULLL_AbilityObjectDataAsset>(BaseObjectDataAsset);

	OverlapCollisionSphere->SetSphereRadius(AbilityObjectDataAsset->OverlapCollisionRadius);

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]{
		FTimerHandle DestroyTimerHandle;
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
			Destroy();
		}), AbilityObjectAttributeSet->GetDestroyTimer(), false);
	}));
}

void ALLL_AbilityObject::DamageTo(AActor* OtherActor)
{
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, OtherActor]{
		const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor);
		if (AbilitySystemInterface && OtherActor != GetOwner())
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			EffectContextHandle.AddInstigator(GetOwner(), this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(AbilityObjectDataAsset->DamageEffect, AbilityLevel, EffectContextHandle);
			if(EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, OffencePower);
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
			}
		}
	}));
}

void ALLL_AbilityObject::KnockBackTo(AActor* OtherActor)
{
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, OtherActor]{
		ILLL_KnockBackInterface* KnockBackInterface = Cast<ILLL_KnockBackInterface>(OtherActor);
		if (KnockBackInterface && OtherActor != GetOwner())
		{
			FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(KnockBackDirection, KnockBackPower);
			KnockBackInterface->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
		}
	}));
}

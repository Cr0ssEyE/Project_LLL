﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_OnAttackHit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "Entity/Object/Thrown/LLL_ThrownFeather.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"
#include "Util/LLL_AbilityDataHelper.h"

ULLL_PGA_OnAttackHit::ULLL_PGA_OnAttackHit() :
bUseOnAttackHitEffect(false),
bUseOnAttackHitSpawnObject(false),
AbilityObjectLocationTarget(EEffectApplyTarget::Target),
bUseOnAttackHitSpawnThrown(false),
ThrowSpeed(0.f),
SpawnOffsetTime(0.f)
{
	
}

void ULLL_PGA_OnAttackHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerRequiredTag.IsValid() && !TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(CurrentEventData.TargetData, 0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (bUseOnAttackHitEffect && IsValid(OnAttackHitEffect))
	{
		ApplyEffectWhenHit();
	}

	if (bUseOnAttackHitSpawnObject && IsValid(AbilityObjectClass))
	{
		SpawnObjectWhenHit();
	}

	if (bUseOnAttackHitSpawnThrown && IsValid(ThrownObjectClass))
	{
		SpawnThrownWhenHit();
	}
}

void ULLL_PGA_OnAttackHit::ApplyEffectWhenHit()
{
	const ULLL_ExtendedGameplayEffect* Effect = Cast<ULLL_ExtendedGameplayEffect>(OnAttackHitEffect.GetDefaultObject());
	const FGameplayEffectSpecHandle EffectHandle = MakeOutgoingGameplayEffectSpec(OnAttackHitEffect, GetAbilityLevel());

	const float ChangeableValue = (AbilityData->AbilityValue + AbilityData->ChangeValue * GetAbilityLevel()) / static_cast<uint32>(AbilityData->AbilityValueType);
	EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_CHANGEABLE_VALUE, ChangeableValue);

	const float UnChangeableValue = AbilityData->UnchangeableValue;
	EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_UNCHANGEABLE_VALUE, UnChangeableValue);
	
	if (AbilityTags.HasTag(TAG_GAS_ABNORMAL_STATUS))
	{
		FLLL_AbilityDataHelper::SetAbnormalStatusAbilityDuration(this, EffectHandle.Data);
	}
	
	if (Effect->GetEffectApplyTarget() == EEffectApplyTarget::Self)
	{
		K2_ApplyGameplayEffectSpecToOwner(EffectHandle);
	}
	else
	{
		K2_ApplyGameplayEffectSpecToTarget(EffectHandle, CurrentEventData.TargetData);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_PGA_OnAttackHit::SpawnObjectWhenHit()
{
	FLLL_AbilityDataHelper::SpawnAbilityObject(this, AbilityObjectClass, CurrentEventData, AbilityObjectLocationTarget);
}

void ULLL_PGA_OnAttackHit::SpawnThrownWhenHit()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());

	int32 SpawnCount = AbilityData->UnchangeableValue == 0 ? 1 : AbilityData->UnchangeableValue;
	if (ThrownObjectClass->IsChildOf(ALLL_ThrownFeather::StaticClass()))
	{
		const UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
		TArray<FActiveGameplayEffectHandle> EffectHandles = ASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(TAG_GAS_ABILITY_PART_COMMON));
		for (const auto EffectHandle : EffectHandles)
		{
			const ULLL_ExtendedGameplayEffect* ActiveEffect = Cast<ULLL_ExtendedGameplayEffect>(ASC->GetActiveGameplayEffect(EffectHandle)->Spec.Def);
			if (!IsValid(ActiveEffect))
			{
				continue;
			}

			if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_FEATHER_AMPLIFICATION))
			{
				SpawnCount += ActiveEffect->GetAbilityData()->AbilityValue;
			}
		}
	}

	float TempSpawnOffsetTime = 0.01f;
	for (int i = 0; i < SpawnCount; i++)
	{
		const AActor* Target = CurrentEventData.TargetData.Data[0]->GetActors()[0].Get();
		
		FTimerHandle SpawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, Player, Target, i, SpawnCount]{
			FVector Location = Player->GetActorLocation();
			FRotator Rotator = FRotationMatrix::MakeFromX(Target->GetActorLocation() - Player->GetActorLocation()).Rotator();

			if (ThrownObjectClass->IsChildOf(ALLL_ThrownFeather::StaticClass()))
			{
				Location -= Rotator.Vector() * Player->GetCapsuleComponent()->GetScaledCapsuleRadius() * 3.0f;
				Rotator += FRotator(0.0f, 180.0f + (FMath::RandBool() ? 10.0f : -10.0f), 0.0f);
			}
			
			ALLL_ThrownObject* ThrownObject = CastChecked<ALLL_ThrownObject>(Player->GetObjectPoolingComponent()->GetActor(ThrownObjectClass));
			ThrownObject->SetActorLocationAndRotation(Location, Rotator);
			ThrownObject->SetAbilityInfo(AbilityData, GetAbilityLevel());
			ThrownObject->Throw(Player, const_cast<AActor*>(Target), ThrowSpeed);

			if (i == SpawnCount - 1)
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			}
		}), TempSpawnOffsetTime, false);

		TempSpawnOffsetTime += SpawnOffsetTime;
	}
}

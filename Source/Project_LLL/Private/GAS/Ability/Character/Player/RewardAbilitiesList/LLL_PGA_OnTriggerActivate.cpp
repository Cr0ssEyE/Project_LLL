// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_OnTriggerActivate.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "Entity/Object/Thrown/LLL_ThrownFeather.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_OnSkillActivate.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"
#include "Util/LLL_AbilityDataHelper.h"

ULLL_PGA_OnTriggerActivate::ULLL_PGA_OnTriggerActivate() :
	bUseOnAttackHitEffect(false),
	bUseSpawnAbilityObject(false),
	AbilityObjectLocationTarget(EEffectApplyTarget::Target),
	bUseSpawnThrownObject(false),
	ThrowSpeed(0.f),
	SpawnOffsetTime(0.f),
	bUseOnAttackHitGrantTag(false),
	bAdditiveOrSubtract(true)
{
}

void ULLL_PGA_OnTriggerActivate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerRequiredTags.IsValid())
	{
		for (auto TriggerRequiredTagOne : TriggerRequiredTags)
		{
			if (!TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTagOne))
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
				return;
			}
		}
	}

	if (ActorStateRequiredTag.IsValid() && !GetAbilitySystemComponentFromActorInfo_Checked()->HasMatchingGameplayTag(ActorStateRequiredTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(CurrentEventData.TargetData, 0))
	{
		bool Valid = false;
		if (bUseSpawnAbilityObject && IsValid(AbilityObjectClass) && AbilityObjectLocationTarget == EEffectApplyTarget::Self)
		{
			Valid = true;
			SpawnAbilityObject();
		}
		
		if (bUseSpawnThrownObject && IsValid(ThrownObjectClass) && !Cast<ALLL_PlayerBase>(CurrentEventData.Instigator))
		{
			Valid = true;
			SpawnThrownObject();
		}
		
		if (!Valid)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
		return;
	}

	if (bUseOnAttackHitEffect && IsValid(OnAttackHitEffect))
	{
		ApplyEffectWhenHit();
	}

	if (bUseSpawnAbilityObject && IsValid(AbilityObjectClass))
	{
		SpawnAbilityObject();
	}

	if (bUseSpawnThrownObject && IsValid(ThrownObjectClass))
	{
		SpawnThrownObject();
	}

	if (bUseOnAttackHitGrantTag && GrantTagContainer.IsValid())
	{
		GrantTagWhenHit();
	}
}

void ULLL_PGA_OnTriggerActivate::ApplyEffectWhenHit()
{
	const ULLL_ExtendedGameplayEffect* Effect = Cast<ULLL_ExtendedGameplayEffect>(OnAttackHitEffect.GetDefaultObject());
	const FGameplayEffectSpecHandle EffectHandle = MakeOutgoingGameplayEffectSpec(OnAttackHitEffect, GetAbilityLevel());

	const float ChangeableValue = (AbilityData->AbilityValue + AbilityData->ChangeValue * (GetAbilityLevel() - 1)) / static_cast<uint32>(AbilityData->AbilityValueType);
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

void ULLL_PGA_OnTriggerActivate::SpawnAbilityObject()
{
	FLLL_AbilityDataHelper::SpawnAbilityObject(this, AbilityObjectClass, CurrentEventData, AbilityObjectLocationTarget);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_PGA_OnTriggerActivate::SpawnThrownObject()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());

	int32 SpawnCount = AbilityData->UnchangeableValue == 0 ? 1 : AbilityData->UnchangeableValue;
	bool ThrowCircular = false;
	bool Straight = false;
	float KnockBackPower = 0.0f;
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
			else if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CHARGED_FEATHER))
			{
				SpawnCount = Player->GetChargedFeatherCount();
				Player->StartChargeFeather();
			}
			else if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CIRCULAR_FEATHER))
			{
				SpawnCount = 12;
				ThrowCircular = true;
				Straight = true;
				// 추후 데이터화 예정
				KnockBackPower = 300.0f;
			}
		}
	}

	float ThrowAngle = 0.0f;
	float TempSpawnOffsetTime = 0.01f;
	for (int i = 0; i < SpawnCount; i++)
	{
		const AActor* Target = Cast<ALLL_PlayerBase>(CurrentEventData.Instigator) ? CurrentEventData.TargetData.Data[0]->GetActors()[0].Get() : CurrentEventData.Instigator;
		
		FTimerHandle SpawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, Player, Target, i, SpawnCount, ThrowAngle, ThrowCircular, Straight]{
			FVector Location = Player->GetActorLocation();
			FRotator Rotator = FRotationMatrix::MakeFromX(Target->GetActorLocation() - Player->GetActorLocation()).Rotator();

			if (ThrownObjectClass->IsChildOf(ALLL_ThrownFeather::StaticClass()))
			{
				if (ThrowCircular)
				{
					Rotator += FRotator(0.0f, ThrowAngle, 0.0f);
				}
				else
				{
					Location -= Rotator.Vector() * Player->GetCapsuleComponent()->GetScaledCapsuleRadius() * 3.0f;
					Rotator += FRotator(0.0f, 180.0f + CHASE_FEATHER_THROW_ANGLE_OFFSET * (FMath::RandBool() ? 1.0f : -1.0f) * FMath::RandRange(0, 5), 0.0f);
				}
			}
			
			ALLL_ThrownObject* ThrownObject = CastChecked<ALLL_ThrownObject>(Player->GetObjectPoolingComponent()->GetActor(ThrownObjectClass));
			ThrownObject->SetActorLocationAndRotation(Location, Rotator);
			ThrownObject->SetAbilityInfo(AbilityData, GetAbilityLevel());
			ThrownObject->Throw(Player, const_cast<AActor*>(Target), ThrowSpeed, Straight, KnockBackPower);

			if (i == SpawnCount - 1)
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			}
		}), TempSpawnOffsetTime, false);

		TempSpawnOffsetTime += SpawnOffsetTime;
		UE_LOG(LogTemp, Log, TEXT("%f, %f"), TempSpawnOffsetTime, SpawnOffsetTime)
		ThrowAngle += 360.0f / SpawnCount;
	}
}

void ULLL_PGA_OnTriggerActivate::GrantTagWhenHit()
{
	float GrantNum = 1.f;
	if (TagGrantNumTag == TAG_GAS_ABILITY_CHANGEABLE_VALUE)
	{
		GrantNum = AbilityData->AbilityValue + AbilityData->ChangeValue * (GetAbilityLevel() - 1);
	}
	else // TagGrantNumTag == TAG_GAS_ABILITY_UNCHANGEABLE_VALUE
	{
		GrantNum = AbilityData->UnchangeableValue;
	}
	
	if (bAdditiveOrSubtract) // Add
	{
		for (auto Actor : CurrentEventData.TargetData.Data[0]->GetActors())
		{
			if (const IAbilitySystemInterface* ASC = Cast<IAbilitySystemInterface>(Actor))
			{
				ASC->GetAbilitySystemComponent()->AddLooseGameplayTags(GrantTagContainer, GrantNum);
			}
		}
	}
	else // Subtract
	{
		for (auto Actor : CurrentEventData.TargetData.Data[0]->GetActors())
		{
			if (const IAbilitySystemInterface* ASC = Cast<IAbilitySystemInterface>(Actor))
			{
				ASC->GetAbilitySystemComponent()->RemoveLooseGameplayTags(GrantTagContainer, GrantNum);
			}
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

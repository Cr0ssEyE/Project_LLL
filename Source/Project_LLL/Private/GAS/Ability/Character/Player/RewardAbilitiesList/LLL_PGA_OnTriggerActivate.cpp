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
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "GAS/Effect/LLL_GE_GiveAbilityComponent.h"
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

	const float MagnitudeValue1 = AbilityData->AbilityValue1 * GetAbilityLevel() / static_cast<uint32>(AbilityData->Value1Type);
	EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_1, MagnitudeValue1);
	
	const float MagnitudeValue2 = AbilityData->AbilityValue2 * GetAbilityLevel() / static_cast<uint32>(AbilityData->Value2Type);
	EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_2, MagnitudeValue2);
	
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
	FLLL_AbilityDataHelper::SpawnAbilityObject(this, AbilityObjectClass, CurrentEventData, AbilityObjectLocationTarget, AbilityObjectOffsetLocation);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_PGA_OnTriggerActivate::SpawnThrownObject()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());

	int32 SpawnCount = AbilityData->AbilityValue1 == 0 ? 1 : AbilityData->AbilityValue1;
	bool ThrowCircular = false;
	bool Straight = false;
	float KnockBackPower = 0.0f;
	if (ThrownObjectClass->IsChildOf(ALLL_ThrownFeather::StaticClass()))
	{
		const UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
		TArray<FActiveGameplayEffectHandle> EffectHandles = ASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(TAG_GAS_ABILITY_NESTING_DENY));
		for (const auto EffectHandle : EffectHandles)
		{
			const ULLL_ExtendedGameplayEffect* ActiveEffect = Cast<ULLL_ExtendedGameplayEffect>(ASC->GetActiveGameplayEffect(EffectHandle)->Spec.Def);
			if (!IsValid(ActiveEffect))
			{
				continue;
			}

			const ULLL_GE_GiveAbilityComponent* GiveAbilityComponent = CastChecked<ULLL_GE_GiveAbilityComponent>(ActiveEffect->FindComponent(ULLL_GE_GiveAbilityComponent::StaticClass()));
			for (auto AbilitySpecConfig : const_cast<ULLL_GE_GiveAbilityComponent*>(GiveAbilityComponent)->GetAbilitySpecConfigs())
			{
				if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilitySpecConfig.Ability))
				{
					if (CastChecked<ULLL_PGA_RewardAbilityBase>(Spec->GetPrimaryInstance()) != this)
					{
						continue;
					}
					
					if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CHARGED_FEATHER))
					{
						SpawnCount = Player->GetChargedFeatherCount();
						Player->StartChargeFeather(AbilityData->AbilityValue1);
					}
					else if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_RANGED_FEATHER))
					{
						SpawnCount = 1;
					}
					else if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CIRCULAR_FEATHER))
					{
						SpawnCount = AbilityData->AbilityValue1;
						ThrowCircular = true;
						Straight = true;
						const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(ASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
						KnockBackPower = AbilityData->KnockBackPower + PlayerCharacterAttributeSet->GetKnockBackPower() - Player->GetOriginKnockBackPower();
					}
				}
			}
		}
	}

	float ThrowCircularAngle = 0.0f;
	float TempSpawnOffsetTime = Player->GetFeatherSpawnStartTime();
	for (int i = 0; i < SpawnCount; i++)
	{
		TArray<AActor*> Targets = Player->GetRangeFeatherTargetsAndClear();
		const AActor* TargetByAttack = Cast<ALLL_PlayerBase>(CurrentEventData.Instigator) ? CurrentEventData.TargetData.Data[0]->GetActors()[0].Get() : CurrentEventData.Instigator;
		Targets.Emplace(const_cast<AActor*>(TargetByAttack));
		if (Targets.Num() == 0)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			return;
		}

		FTimerHandle SpawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, Player, Targets, i, SpawnCount, ThrowCircularAngle, ThrowCircular, Straight, KnockBackPower]{
			for (const auto Target : Targets)
			{
				if (!Straight && !IsValid(Target))
				{
					EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
					return;
				}
				
				FVector Location = Player->GetActorLocation();
				FRotator Rotator;

				if (!Straight)
				{
					Rotator = FRotationMatrix::MakeFromX(Target->GetActorLocation() - Player->GetActorLocation()).Rotator();
				}
				else
				{
					Rotator = FRotationMatrix::MakeFromX(Player->GetActorForwardVector()).Rotator();
				}

				ALLL_ThrownObject* ThrownObject = CastChecked<ALLL_ThrownObject>(Player->GetObjectPoolingComponent()->GetActor(ThrownObjectClass));
				if (ThrowCircular)
				{
					Rotator += FRotator(0.0f, ThrowCircularAngle, 0.0f);
				}
				else
				{
					if (const ALLL_ThrownFeather* ThrownFeather = Cast<ALLL_ThrownFeather>(ThrownObject))
					{
						const float Offset = ThrownFeather->GetChaseFeatherThrowAngleOffset();
						const int32 Multiply = ThrownFeather->GetChaseFeatherThrowAngleRandomMultiply();
						
						Location -= Rotator.Vector() * Player->GetCapsuleComponent()->GetScaledCapsuleRadius() * 3.0f;
						Rotator += FRotator(0.0f, 180.0f + Offset * (FMath::RandBool() ? 1.0f : -1.0f) * FMath::RandRange(0, Multiply), 0.0f);
					}
				}
				ThrownObject->SetActorLocationAndRotation(Location, Rotator);
				ThrownObject->SetAbilityInfo(AbilityData, GetAbilityLevel());
				ThrownObject->Throw(Player, Target, ThrowSpeed, Straight, KnockBackPower);
			}

			if (i == SpawnCount - 1)
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			}
		}), TempSpawnOffsetTime, false);

		TempSpawnOffsetTime += SpawnOffsetTime;
		ThrowCircularAngle += 360.0f / SpawnCount;
	}
}

void ULLL_PGA_OnTriggerActivate::GrantTagWhenHit()
{
	float GrantNum;
	if (TagGrantNumTag == TAG_GAS_ABILITY_VALUE_1)
	{
		GrantNum = AbilityData->AbilityValue1 * GetAbilityLevel();
	}
	else if (TagGrantNumTag == TAG_GAS_ABILITY_VALUE_2)
	{
		GrantNum = AbilityData->AbilityValue2 * GetAbilityLevel();
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

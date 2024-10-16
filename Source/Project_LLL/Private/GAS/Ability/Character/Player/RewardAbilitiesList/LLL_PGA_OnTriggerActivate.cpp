// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_OnTriggerActivate.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "Entity/Object/Thrown/LLL_ThrownFeather.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "GAS/Effect/LLL_GE_GiveAbilityComponent.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"
#include "Util/LLL_AbilityDataHelper.h"

ULLL_PGA_OnTriggerActivate::ULLL_PGA_OnTriggerActivate() :
	bUseApplyEffect(false),
	bUseSpawnAbilityObject(false),
	AbilityObjectLocationTarget(EEffectApplyTarget::Target),
	bUseSpawnThrownObject(false),
	ThrowSpeed(0.f),
	SpawnOffsetTime(0.f)
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
		if (bUseApplyEffect && IsValid(ApplyEffect))
		{
			Valid = true;
			ApplyEffectWhenHit();
		}
		
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

	if (bUseApplyEffect && IsValid(ApplyEffect))
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
}

void ULLL_PGA_OnTriggerActivate::ApplyEffectWhenHit()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	
	ULLL_ExtendedGameplayEffect* Effect = Cast<ULLL_ExtendedGameplayEffect>(ApplyEffect.GetDefaultObject());
	const FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(ApplyEffect, GetAbilityLevel());

	const float MagnitudeValue1 = AbilityData->AbilityValue1 * GetAbilityLevel() / static_cast<uint32>(AbilityData->Value1Type);
	const float MagnitudeValue2 = AbilityData->AbilityValue2 * GetAbilityLevel() / static_cast<uint32>(AbilityData->Value2Type);
	
	EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_1, MagnitudeValue1);

	// 재빠른 몸놀림 이누리아
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_EVASION_DASH) && AbilityTags.HasTag(TAG_GAS_EVASION) && Player->GetEnuriaCount(EAnimalType::Horse) >= Player->GetEvasionDashHorseEnuriaCheckCount())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_2, 1.0f);
	}
	else
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_2, MagnitudeValue2);
	}
	EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_HUNDRED_VALUE_1, MagnitudeValue1 * 100.0f);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_HUNDRED_VALUE_2, MagnitudeValue2 * 100.0f);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_MINUS_VALUE_1, MagnitudeValue1 * -1.0f);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_MINUS_VALUE_2, MagnitudeValue2 * -1.0f);
	
	if (AbilityTags.HasTag(TAG_GAS_BLEEDING))
	{
		FLLL_AbilityDataHelper::SetBleedingPeriodValue(Player, Effect);
	}
	
	if (Effect->GetEffectApplyTarget() == EEffectApplyTarget::Self)
	{
		PlayerASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
	else
	{
		for (auto Target : CurrentEventData.TargetData.Data[0]->GetActors())
		{
			if (ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Target.Get()))
			{
				if (AbilityTags.HasTag(TAG_GAS_BLEEDING))
				{
					if (!Monster->GetBleedingTrigger())
					{
						Monster->ToggleBleedingTrigger();
						continue;
					}
					Monster->ToggleBleedingTrigger();
					
					if (FLLL_AbilityDataHelper::CheckBleedingExplosion(Player, Monster, Player))
					{
						continue;
					}
				}
				PlayerASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Monster->GetAbilitySystemComponent());
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_PGA_OnTriggerActivate::SpawnAbilityObject()
{
	FLLL_AbilityDataHelper::SpawnAbilityObject(this, AbilityObjectClass, CurrentEventData, AbilityObjectLocationTarget, AbilityObjectOffsetLocation, AbilityObjectOffsetRotation);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_PGA_OnTriggerActivate::SpawnThrownObject()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());

	int32 SpawnCount = 1;
	bool ThrowCircular = false;
	bool Straight = true;
	float KnockBackPower = 0.0f;
	if (ThrownObjectClass->IsChildOf(ALLL_ThrownFeather::StaticClass()))
	{
		SpawnCount = AbilityData->AbilityValue1 == 0 ? 1 : AbilityData->AbilityValue1;
		Straight = false;
		
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

					// 깃털 연발 이누리아
					if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CHARGED_FEATHER))
					{
						SpawnCount = Player->GetChargedFeatherCount();
						if (SpawnCount >= 1)
						{
							Player->StartChargeFeather(AbilityData->AbilityValue1);
						}
					}
					// 화려한 등장 이누리아
					else if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_RANGED_FEATHER))
					{
						SpawnCount = 1;
					}
					// 접근 금지 이누리아
					else if (ActiveEffect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CIRCULAR_FEATHER))
					{
						SpawnCount = AbilityData->AbilityValue1;
						ThrowCircular = true;
						Straight = true;
						KnockBackPower = AbilityData->KnockBackPower;
					}
				}
			}
		}
	}

	float ThrowCircularAngle = 0.0f;
	float TempSpawnOffsetTime = Player->GetFeatherSpawnStartTime();

	if (SpawnCount == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	for (int i = 0; i < SpawnCount; i++)
	{
		TArray<AActor*> Targets = Player->GetRangeFeatherTargetsAndClear();
		const AActor* TargetByAttack = Cast<ALLL_PlayerBase>(CurrentEventData.Instigator) ? CurrentEventData.TargetData.Data[0]->GetActors()[0].Get() : CurrentEventData.Instigator;
		Targets.Emplace(const_cast<AActor*>(TargetByAttack));
		if (Targets.Num() == 0)
		{
			if (Straight)
			{
				Targets.Emplace(nullptr);
			}
			else
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
				return;
			}
		}

		FTimerHandle SpawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, Player, Targets, i, SpawnCount, ThrowCircularAngle, ThrowCircular, Straight, KnockBackPower]{
			for (const auto Target : Targets)
			{
				if (!Straight && !Cast<ALLL_MonsterBase>(Target))
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_SpawnThrownObject.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_ThrownFeather.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"

void ULLL_PGA_SpawnThrownObject::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

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
		const AActor* Target = bTargetIsInstigator ? CurrentEventData.Instigator : CurrentEventData.TargetData.Data[0]->GetActors()[0].Get();
		
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
			ThrownObject->Throw(Player, const_cast<AActor*>(Target), Speed);

			if (i == SpawnCount - 1)
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			}
		}), TempSpawnOffsetTime, false);

		TempSpawnOffsetTime += SpawnOffsetTime;
	}
}

﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/LLL_MGA_SetFallableState.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitOverlap.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Task/LLL_AT_WaitOverlap.h"
#include "Util/LLL_MathHelper.h"

bool ULLL_MGA_SetFallableState::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	if (Monster->GetCapsuleComponent()->GetCollisionProfileName() == CP_MONSTER_FALLABLE)
	{
		return false;
	}

	return Result;
}

void ULLL_MGA_SetFallableState::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER_FALLABLE);
	const float KnockBackTime = FLLL_MathHelper::CalculatePlayerKnockBackCollisionCheckEndApproximation(Monster->GetKnockBackedPower());
	
	FTimerHandle CollisionRestoreHandle;
	GetWorld()->GetTimerManager().SetTimer(CollisionRestoreHandle, FTimerDelegate::CreateWeakLambda(this, [&]()
	{
		if (IsValid(GetAvatarActorFromActorInfo()) && IsValid(this))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}), KnockBackTime, false);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckMonsterCollisionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("낙하 상태 어빌리티 발동 %s"), *CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo())->GetCapsuleComponent()->GetCollisionProfileName().ToString()));
		}
	}
#endif
	ULLL_AT_WaitOverlap* OverlapTask = ULLL_AT_WaitOverlap::WaitForOverlap(this, false);
	OverlapTask->OnOverlap.AddDynamic(this, &ULLL_MGA_SetFallableState::OnOverlapCallBack);
	OverlapTask->ReadyForActivation();
}

void ULLL_MGA_SetFallableState::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckMonsterCollisionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("낙하 상태 어빌리티 종료")));
			ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
		}
	}
#endif

	if (bWasCancelled || !IsActive())
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	GetWorld()->SweepSingleByChannel(
		HitResult,
		Monster->GetActorLocation(),
		Monster->GetActorLocation(),
		FQuat::Identity,
		ECC_WALL_ONLY,
		FCollisionShape::MakeSphere(10.f),
		Params
		);

	if (HitResult.GetActor())
	{
		Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_NO_COLLISION);
	}
	else
	{
		Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
	}
	
	GetAbilitySystemComponentFromActorInfo_Checked()->RemoveLooseGameplayTag(TAG_GAS_MONSTER_FALLABLE);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_MGA_SetFallableState::OnOverlapCallBack(const FGameplayAbilityTargetDataHandle& TargetData)
{
	for (auto Actor : TargetData.Data[0]->GetActors())
	{
		for (auto Component : Actor->GetComponents())
		{
			UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component);
			if (!IsValid(StaticMeshComponent))
			{
				continue;
			}

			if (StaticMeshComponent->GetCollisionProfileName() == CP_INVISIBLE_WALL)
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
				return;
			}
		}
	}
}
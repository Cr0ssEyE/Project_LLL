// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/MeleeMonster/SwordDash/LLL_MGA_Dash.h"

#include "FMODAudioComponent.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Interface/LLL_DashMonsterInterface.h"
#include "Util/LLL_FModPlayHelper.h"

void ULLL_MGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER_DASH);
	Monster->GetMovementComponent()->Velocity = FVector::Zero();
	
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	FHitResult StaticResult;
	FHitResult PlayerResult;
	const FVector SweepStartLocation = Monster->GetActorLocation();
	const FVector SweepEndLocation = SweepStartLocation + Monster->GetActorForwardVector() * MonsterAttributeSet->GetMonsterData1();
	const FQuat SweepQuat = Monster->GetActorQuat();
	constexpr ECollisionChannel StaticTraceChannel = ECC_WALL_ONLY;
	constexpr ECollisionChannel PlayerTraceChannel = ECC_PLAYER_CHECK;

	const UCapsuleComponent* Capsule = Monster->GetCapsuleComponent();
	const FCollisionShape TraceShape = FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight() / 2.0f);
		
	GetWorld()->SweepSingleByChannel(
		StaticResult,
		SweepStartLocation,
		SweepEndLocation,
		SweepQuat,
		StaticTraceChannel,
		TraceShape);
	
	GetWorld()->SweepSingleByChannel(
		PlayerResult,
		SweepStartLocation,
		SweepEndLocation,
		SweepQuat,
		PlayerTraceChannel,
		TraceShape);

	FVector DashLocation = SweepEndLocation;
	if (StaticResult.GetActor())
	{
		if (PlayerResult.GetActor())
		{
			const float StaticDistance = Monster->GetDistanceTo(StaticResult.GetActor());
			const float PlayerDistance = Monster->GetDistanceTo(PlayerResult.GetActor());

			DashLocation = (StaticDistance <= PlayerDistance) ? StaticResult.Location : PlayerResult.Location;

			// Todo : 최소 돌진 거리 처리 로직 구현 필요
		}
		else
		{
			DashLocation = StaticResult.Location;
		}
	}
	else if (PlayerResult.GetActor())
	{
		DashLocation = PlayerResult.Location;
	}

	ILLL_DashMonsterInterface* DashMonster = CastChecked<ILLL_DashMonsterInterface>(Monster);
	DashMonster->SetDashing(true);

	UAbilityTask_MoveToLocation* MoveToLocationTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("Dash"), DashLocation, 0.1f / Monster->CustomTimeDilation, nullptr, nullptr);
	MoveToLocationTask->OnTargetLocationReached.AddDynamic(this, &ULLL_MGA_Dash::OnCompleteCallBack);
	MoveToLocationTask->ReadyForActivation();

	BP_ApplyGameplayEffectToOwner(DashEffect);
}

void ULLL_MGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);

	ILLL_DashMonsterInterface* DashMonster = CastChecked<ILLL_DashMonsterInterface>(Monster);
	DashMonster->SetDashing(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

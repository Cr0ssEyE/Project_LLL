// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/Base/LLL_MGA_Dash.h"

#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Interface/LLL_DashMonsterInterface.h"

void ULLL_MGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER_DASH);
	Monster->GetMesh()->SetCollisionProfileName(CP_MONSTER_DASH);
	Monster->GetMovementComponent()->Velocity = FVector::Zero();
	
	ILLL_DashMonsterInterface* DashMonster = CastChecked<ILLL_DashMonsterInterface>(Monster);
	DashMonster->SetDashing(true);

	FHitResult StaticResult;
	FHitResult PlayerResult;
	const FVector SweepStartLocation = Monster->GetActorLocation();
	const FVector SweepEndLocation = SweepStartLocation + Monster->GetActorForwardVector() * DashMonster->GetMaxDashDistance();
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
		if (PlayerResult.GetActor() && !bPassPlayer)
		{
			const float StaticDistance = Monster->GetDistanceTo(StaticResult.GetActor());
			const float PlayerDistance = Monster->GetDistanceTo(PlayerResult.GetActor());

			if (StaticDistance <= PlayerDistance)
			{
				DashLocation = StaticResult.Location;
			}
			else if (PlayerDistance < DashMonster->GetMinDashDistance())
			{
				DashLocation = SweepStartLocation + Monster->GetActorForwardVector() * DashMonster->GetMinDashDistance();
			}
			else
			{
				DashLocation = PlayerResult.Location;
			}
		}
		else
		{
			DashLocation = StaticResult.Location;
		}
	}
	else if (PlayerResult.GetActor() && !bPassPlayer)
	{
		const float PlayerDistance = Monster->GetDistanceTo(PlayerResult.GetActor());
		if (PlayerDistance < DashMonster->GetMinDashDistance())
		{
			DashLocation = SweepStartLocation + Monster->GetActorForwardVector() * DashMonster->GetMinDashDistance();
		}
		else
		{
			DashLocation = PlayerResult.Location;
		}
	}

	UAbilityTask_MoveToLocation* MoveToLocationTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("Dash"), DashLocation, DashMonster->GetMaxDashDistance() / DashMonster->GetDashSpeed() / Monster->CustomTimeDilation, nullptr, nullptr);
	MoveToLocationTask->OnTargetLocationReached.AddDynamic(this, &ULLL_MGA_Dash::OnCompleteCallBack);
	MoveToLocationTask->ReadyForActivation();

	if (IsValid(DashEffect))
	{
		BP_ApplyGameplayEffectToOwner(DashEffect);
	}
}

void ULLL_MGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	if (Cast<ALLL_BossMonster>(Monster))
	{
		Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_BOSS);
		Monster->GetMesh()->SetCollisionProfileName(CP_BOSS);
	}
	else
	{
		Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
		Monster->GetMesh()->SetCollisionProfileName(CP_MONSTER);
	}

	ILLL_DashMonsterInterface* DashMonster = CastChecked<ILLL_DashMonsterInterface>(Monster);
	DashMonster->SetDashing(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

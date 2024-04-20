// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/MeleeMonster/SwordDash/LLL_MGA_Dash.h"

#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/SwordDash/LLL_SwordDashAttributeSet.h"
#include "Interface/LLL_DashMonsterInterface.h"

void ULLL_MGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
	Monster->GetMovementComponent()->Velocity = FVector::Zero();

	const ULLL_SwordDashAttributeSet* SwordDashAttributeSet = CastChecked<ULLL_SwordDashAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_SwordDashAttributeSet::StaticClass()));

	FHitResult StaticResult;
	FHitResult PlayerResult;
	const FVector SweepStartLocation = Monster->GetActorLocation();
	FVector SweepEndLocation = SweepStartLocation + Monster->GetActorForwardVector() * SwordDashAttributeSet->GetDashDistance();
	FQuat SweepQuat = Monster->GetActorQuat();
	ECollisionChannel StaticTraceChannel = ECC_WALL_ONLY;
	ECollisionChannel PlayerTraceChannel = ECC_PLAYER;

	UCapsuleComponent* Capsule = Monster->GetCapsuleComponent();
	FCollisionShape TraceShape = FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight() / 2.0f);
		
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
			float StaticDistance = Monster->GetDistanceTo(StaticResult.GetActor());
			float PlayerDistance = Monster->GetDistanceTo(PlayerResult.GetActor());

			DashLocation = (StaticDistance <= PlayerDistance) ? StaticResult.Location : PlayerResult.Location;
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
	DashMonster->SetDash(true);

	UAbilityTask_MoveToLocation* MoveToLocationTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("Dash"), DashLocation, 0.1f, nullptr, nullptr);
	MoveToLocationTask->OnTargetLocationReached.AddDynamic(this, &ULLL_MGA_Dash::OnCompleteCallBack);

	MoveToLocationTask->ReadyForActivation();
}

void ULLL_MGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);

	ILLL_DashMonsterInterface* DashMonster = CastChecked<ILLL_DashMonsterInterface>(Monster);
	DashMonster->SetDash(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

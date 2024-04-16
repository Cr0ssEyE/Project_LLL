// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/MeleeMonster/SwordDash/LLL_MGA_Dash.h"

#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/SwordDash/LLL_SwordDashAttributeSet.h"

void ULLL_MGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_SwordDash* SwordDash = CastChecked<ALLL_SwordDash>(GetAvatarActorFromActorInfo());
	SwordDash->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
	SwordDash->GetMovementComponent()->Velocity = FVector::Zero();
	
	const ULLL_SwordDashAttributeSet* SwordDashAttributeSet = CastChecked<ULLL_SwordDashAttributeSet>(SwordDash->GetAbilitySystemComponent()->GetAttributeSet(ULLL_SwordDashAttributeSet::StaticClass()));

	FHitResult StaticResult;
	FHitResult PlayerResult;
	const FVector SweepStartLocation = SwordDash->GetActorLocation();
	FVector SweepEndLocation = SweepStartLocation + SwordDash->GetActorForwardVector() * SwordDashAttributeSet->GetMaxDashDistance();
	FQuat SweepQuat = SwordDash->GetActorQuat();
	ECollisionChannel StaticTraceChannel = ECC_WALL_ONLY;
	ECollisionChannel PlayerTraceChannel = ECC_PLAYER;

	UCapsuleComponent* Capsule = SwordDash->GetCapsuleComponent();
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
			float StaticDistance = SwordDash->GetDistanceTo(StaticResult.GetActor());
			float PlayerDistance = SwordDash->GetDistanceTo(PlayerResult.GetActor());

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
	
	SwordDash->SetDash(true);

	UAbilityTask_MoveToLocation* MoveToLocationTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("Dash"), DashLocation, 0.1f, nullptr, nullptr);
	MoveToLocationTask->OnTargetLocationReached.AddDynamic(this, &ULLL_MGA_Dash::OnCompleteCallBack);

	MoveToLocationTask->ReadyForActivation();
}

void ULLL_MGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_SwordDash* SwordDash = CastChecked<ALLL_SwordDash>(GetAvatarActorFromActorInfo());
	SwordDash->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
	SwordDash->SetDash(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

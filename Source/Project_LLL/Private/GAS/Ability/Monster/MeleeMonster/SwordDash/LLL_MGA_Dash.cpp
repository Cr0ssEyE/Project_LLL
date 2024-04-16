// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/MeleeMonster/SwordDash/LLL_MGA_Dash.h"

#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/SwordDash/LLL_SwordDashAttributeSet.h"
#include "Interface/LLL_DashMonsterInterface.h"

void ULLL_MGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_SwordDash* SwordDash = CastChecked<ALLL_SwordDash>(GetAvatarActorFromActorInfo());
	SwordDash->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
	SwordDash->GetMovementComponent()->Velocity = FVector::Zero();
	
	const ULLL_SwordDashAttributeSet* SwordDashAttributeSet = CastChecked<ULLL_SwordDashAttributeSet>(SwordDash->GetAbilitySystemComponent()->GetAttributeSet(ULLL_SwordDashAttributeSet::StaticClass()));
	//Monster->LaunchCharacter(Monster->GetActorForwardVector() * MonsterAttributeSet->GetAttackDistance(), true, true);

	FHitResult Result;
	const FVector SweepStartLocation =  SwordDash->GetActorLocation();
	const FVector SweepEndLocation = SweepStartLocation + SwordDash->GetActorForwardVector() * SwordDashAttributeSet->GetMaxDashDistance();
	FQuat SweepQuat = SwordDash->GetActorQuat();
	ECollisionChannel TraceChannel = ECC_WALL_ONLY;

	UCapsuleComponent* Capsule = SwordDash->GetCapsuleComponent();
	float x = Capsule->GetScaledCapsuleRadius() * 2.0f;
	float y = Capsule->GetScaledCapsuleHalfHeight() * 2.0f;
	float z = SwordDashAttributeSet->GetMaxDashDistance();
	FVector BoxExtents = FVector(x, y, z);
	FCollisionShape TraceShape = FCollisionShape::MakeBox(BoxExtents);
		
	GetWorld()->SweepSingleByChannel(
		Result,
		SweepStartLocation,
		SweepEndLocation,
		SweepQuat,
		TraceChannel,
		TraceShape);

	FVector DashLocation = SweepEndLocation;
	if (AActor* HitActor = Result.GetActor())
	{
		DashLocation = Result.Location;
	}
	SwordDash->SetActorLocation(DashLocation);
	
	ILLL_DashMonsterInterface* DashMonster = CastChecked<ILLL_DashMonsterInterface>(SwordDash);
	DashMonster->SetDash(true);
	
	FTimerHandle HideTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this, &ULLL_MGA_Dash::OnCompleteCallBack, 0.2f, false);
}

void ULLL_MGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
	
	ILLL_DashMonsterInterface* DashMonster = CastChecked<ILLL_DashMonsterInterface>(Monster);
	DashMonster->SetDash(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

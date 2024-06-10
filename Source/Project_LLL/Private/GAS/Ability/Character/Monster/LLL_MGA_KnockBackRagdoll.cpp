// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/LLL_MGA_KnockBackRagdoll.h"

#include "AbilitySystemComponent.h"
#include "BrainComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

ULLL_MGA_KnockBackRagdoll::ULLL_MGA_KnockBackRagdoll() :
	RagdollStateDuration(1.f),
	CurrentBlendWeight(1.f)
{
	
}

void ULLL_MGA_KnockBackRagdoll::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	if (!IsValid(TriggerEventData->Instigator))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void ULLL_MGA_KnockBackRagdoll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = CastChecked<ULLL_MonsterBaseAnimInstance>(Monster->GetMesh()->GetAnimInstance());
	MonsterBaseAnimInstance->StopAllMontages(1.f);
	MonsterBaseAnimInstance->Montage_Play(RagdollBeginMontage);
	FVector HitDirection = (Monster->GetActorLocation() - Monster->GetLastCollideLocation()).GetSafeNormal2D();
	HitDirection.Z = 0.f;
	Monster->SetActorRotation(HitDirection.Rotation(), ETeleportType::TeleportPhysics);
	// Monster->SetActorLocation(Monster->GetActorLocation() - HitDirection * 50.f);
	
	Monster->GetCharacterMovement()->Velocity = FVector::Zero();
	Monster->GetMesh()->SetCollisionProfileName(CP_RAGDOLL);
	Monster->GetMesh()->GetBodyInstance()->bLockXTranslation = true;
	Monster->GetMesh()->GetBodyInstance()->bLockYTranslation = true;
	Monster->GetMesh()->SetAllBodiesPhysicsBlendWeight(1.f);
	Monster->GetMesh()->SetAllPhysicsLinearVelocity(FVector::Zero());
	Monster->GetMesh()->SetLinearDamping(1.f);
	Monster->GetMesh()->SetSimulatePhysics(true);
	
	const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(Monster->GetController());
	MonsterBaseAIController->GetBrainComponent()->StopLogic("Monster Is Collide To Wall");

	if (GetWorld()->GetTimerManager().IsTimerActive(RagdollTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RagdollTimerHandle);
	}

	CurrentBlendWeight = 1.f;
	GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &ULLL_MGA_KnockBackRagdoll::RestoreRagdollState, RagdollStateDuration, false);
}

void ULLL_MGA_KnockBackRagdoll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsValid(GetWorld()))
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}
	CurrentBlendWeight = 1.f;
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetMesh()->SetAllPhysicsLinearVelocity(FVector::Zero());
	Monster->GetMesh()->SetLinearDamping(0.01f);
	Monster->GetMesh()->SetSimulatePhysics(false);
	if (Monster->GetMesh()->GetBodyInstance())
	{
		Monster->GetMesh()->GetBodyInstance()->bLockXTranslation = false;
		Monster->GetMesh()->GetBodyInstance()->bLockYTranslation = false;
	}
	Monster->GetMesh()->SetCollisionProfileName(CP_MONSTER);
	Monster->GetMesh()->SetAllBodiesPhysicsBlendWeight(1.f);
	
	if (IsValid(Monster->GetController()))
	{
		const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(Monster->GetController());
		MonsterBaseAIController->GetBrainComponent()->RestartLogic();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_MGA_KnockBackRagdoll::RestoreRagdollState()
{
	if (CurrentBlendWeight <= 0.f)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	CurrentBlendWeight -= GetWorld()->DeltaTimeSeconds;
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->GetMesh()->SetAllBodiesPhysicsBlendWeight(CurrentBlendWeight);
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, & ULLL_MGA_KnockBackRagdoll::RestoreRagdollState);
}

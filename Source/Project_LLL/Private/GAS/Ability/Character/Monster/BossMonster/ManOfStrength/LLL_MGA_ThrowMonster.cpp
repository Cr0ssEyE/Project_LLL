﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/BossMonster/ManOfStrength/LLL_MGA_ThrowMonster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Util/LLL_MathHelper.h"

void ULLL_MGA_ThrowMonster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(Monster->GetController());
	ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_OTHER_MONSTER));
	if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
	{
		const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(Player->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

		const float PredictionRate = MonsterAttributeSet->GetManOfStrengthPredictionRate();
		const float OffsetZ = Player->GetActorLocation().Z + Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FVector Velocity = Player->GetVelocity();
		if (Player->GetCapsuleComponent()->GetCollisionProfileName() == CP_PLAYER_EVADE)
		{
			Velocity = Player->GetLastVelocityBeforeDash();
		}
		
		FVector PredictedLocation = FLLL_MathHelper::GetPredictedLocation(OtherMonster, Player, Velocity, PlayerAttributeSet->GetMoveSpeed(), PredictionRate);
		PredictedLocation.Z = OffsetZ;
		FVector StartLocation = OtherMonster->GetActorLocation();
		StartLocation.Z = OffsetZ;
		
		FVector Direction = (PredictedLocation - StartLocation).GetSafeNormal();
		Direction = FLLL_MathHelper::CalculatePredictedDirection(Monster, Direction);

		const float Speed = MonsterAttributeSet->GetManOfStrengthThrowSpeed();
		OtherMonster->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		OtherMonster->SetActorLocation(StartLocation);
		CastChecked<ULLL_MonsterBaseAnimInstance>(OtherMonster->GetCharacterAnimInstance())->SetSnapped(false);
		OtherMonster->GetMesh()->SetCollisionProfileName(CP_THREW_MONSTER);
		OtherMonster->GetCapsuleComponent()->SetCollisionProfileName(CP_THREW_MONSTER);
		UCharacterMovementComponent* CharacterMovementComponent = CastChecked<UCharacterMovementComponent>(OtherMonster->GetMovementComponent());
		CharacterMovementComponent->MovementMode = MOVE_Flying;
		CharacterMovementComponent->Velocity = Direction * Speed;
		MonsterAIController->GetBlackboardComponent()->SetValueAsObject(BBKEY_OTHER_MONSTER, nullptr);

		OtherMonster->DisconnectOwnerDeadDelegate();

		UE_LOG(LogTemp, Log, TEXT("%s 던지기"), *OtherMonster->GetName())
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("몬스터 던지기 실패"))
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

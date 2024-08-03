// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/BossMonster/ManOfStrength/LLL_MGA_ThrowMonster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULLL_MGA_ThrowMonster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(Monster->GetController());
	ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_OTHER_MONSTER));
	if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
	{
		FVector FloorStartLocation = OtherMonster->GetActorLocation();
		FloorStartLocation.Z = 0.0f;
		const FVector PlayerLocation = CastChecked<ALLL_PlayerBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER))->GetActorLocation();
		FVector FloorEndLocation = PlayerLocation;
		FloorEndLocation.Z = 0.0f;
		const FVector FloorDirection = (FloorEndLocation - FloorStartLocation).GetSafeNormal();
		const float FloorDistance = FVector::Distance(FloorStartLocation, FloorEndLocation);
		FVector TargetLocation = FloorStartLocation + FloorDirection * FloorDistance * 2.0f;
		TargetLocation.Z = PlayerLocation.Z;
		const FVector Direction = (TargetLocation - OtherMonster->GetActorLocation()).GetSafeNormal();

		// Todo : 추후 데이터화 예정
		float Speed = 3000.0f;
		OtherMonster->SetOwner(nullptr);
		OtherMonster->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		OtherMonster->GetMesh()->SetCollisionProfileName(CP_THREW_MONSTER);
		OtherMonster->GetCapsuleComponent()->SetCollisionProfileName(CP_THREW_MONSTER);
		UCharacterMovementComponent* CharacterMovementComponent = CastChecked<UCharacterMovementComponent>(OtherMonster->GetMovementComponent());
		CharacterMovementComponent->MovementMode = MOVE_Flying;
		CharacterMovementComponent->Velocity = Direction * Speed;
		MonsterAIController->GetBlackboardComponent()->SetValueAsObject(BBKEY_OTHER_MONSTER, nullptr);

		UE_LOG(LogTemp, Log, TEXT("%s 던지기"), *OtherMonster->GetName())
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("몬스터 던지기 실패"))
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

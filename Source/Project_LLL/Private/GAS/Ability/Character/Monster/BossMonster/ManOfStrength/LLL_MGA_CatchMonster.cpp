// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/BossMonster/ManOfStrength/LLL_MGA_CatchMonster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

void ULLL_MGA_CatchMonster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(Monster->GetController());
	ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_OTHER_MONSTER));
	if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
	{
		if (!Monster->GetMesh()->DoesSocketExist(SocketName))
		{
			UE_LOG(LogTemp, Log, TEXT("소켓이 존재하지 않습니다"))
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}

		CastChecked<ALLL_MonsterBaseAIController>(OtherMonster->GetController())->StopLogic(TEXT("Snapped"));
		CastChecked<UCharacterMovementComponent>(OtherMonster->GetMovementComponent())->MovementMode = MOVE_None;
		OtherMonster->GetMesh()->SetCollisionProfileName(CP_NO_COLLISION);
		OtherMonster->GetCapsuleComponent()->SetCollisionProfileName(CP_NO_COLLISION);
		CastChecked<ULLL_MonsterBaseAnimInstance>(OtherMonster->GetCharacterAnimInstance())->SetSnapped(true);
		OtherMonster->AttachToComponent(Monster->GetMesh(), FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false
			), SocketName);
		
		OtherMonster->SetOwner(Monster);
		OtherMonster->ConnectOwnerDeadDelegate();

		UE_LOG(LogTemp, Log, TEXT("%s 잡기"), *OtherMonster->GetName())
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("몬스터 잡기 실패"))
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

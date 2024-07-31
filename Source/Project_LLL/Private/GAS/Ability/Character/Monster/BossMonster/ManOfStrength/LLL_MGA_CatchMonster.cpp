// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/BossMonster/ManOfStrength/LLL_MGA_CatchMonster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

void ULLL_MGA_CatchMonster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(Monster->GetController());
	ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_OTHER_MONSTER));
	if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
	{
		// Todo : 추후 데이터화 예정
		const FName SocketName = TEXT("hand_rSocket");
		if (!Monster->GetMesh()->DoesSocketExist(SocketName))
		{
			UE_LOG(LogTemp, Log, TEXT("소켓이 존재하지 않습니다"))
			return;
		}

		CastChecked<ALLL_MonsterBaseAIController>(OtherMonster->GetController())->StopLogic(TEXT("Snapped"));
		OtherMonster->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OtherMonster->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OtherMonster->AttachToComponent(Monster->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

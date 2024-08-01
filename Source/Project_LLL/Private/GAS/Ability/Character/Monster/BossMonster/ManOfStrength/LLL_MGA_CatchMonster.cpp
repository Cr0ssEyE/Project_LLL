// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/BossMonster/ManOfStrength/LLL_MGA_CatchMonster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

void ULLL_MGA_CatchMonster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(Monster->GetController());
	ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_OTHER_MONSTER));
	if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
	{
		// Todo : 추후 데이터화 예정
		const FName SocketName = TEXT("hand_rSocket");
		if (!Monster->GetMesh()->DoesSocketExist(SocketName))
		{
			UE_LOG(LogTemp, Log, TEXT("소켓이 존재하지 않습니다"))
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		}

		OtherMonster->Snapped();
		OtherMonster->AttachToComponent(Monster->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("몬스터 잡기 실패"))
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UE_LOG(LogTemp, Log, TEXT("%s 잡기"), *OtherMonster->GetName())
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

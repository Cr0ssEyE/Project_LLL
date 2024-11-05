// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Kismet/GameplayStatics.h"

void ALLL_MonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Monster = CastChecked<ALLL_MonsterBase>(InPawn);
	MonsterDataAsset = CastChecked<ULLL_MonsterBaseDataAsset>(Monster->GetCharacterDataAsset());
	
	// 블랙보드와 행동트리 할당
	UBlackboardComponent* NewBlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterDataAsset->BlackBoard, NewBlackboardComponent))
	{
		check(RunBehaviorTree(MonsterDataAsset->BehaviorTree));
		BlackboardComponent = NewBlackboardComponent;
	}

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]{
		Monster->GetCharacterAnimInstance()->OnMontageStarted.AddDynamic(this, &ALLL_MonsterBaseAIController::StartDamagedHandle);
		Monster->GetCharacterAnimInstance()->OnMontageEnded.AddDynamic(this, &ALLL_MonsterBaseAIController::EndDamagedHandle);
	}));
}

void ALLL_MonsterBaseAIController::SetPlayer(ALLL_PlayerBase* Player) const
{
	BlackboardComponent->SetValueAsObject(BBKEY_PLAYER, Player);
}

void ALLL_MonsterBaseAIController::StopLogic(const FString& Reason) const
{
	BrainComponent->StopLogic(Reason);
	
	const FGameplayTagContainer WithOutTags = FGameplayTagContainer(TAG_GAS_ABILITY_NOT_CANCELABLE);
	Monster->GetAbilitySystemComponent()->CancelAbilities(nullptr, &WithOutTags);
}

void ALLL_MonsterBaseAIController::StartLogic() const
{
	BrainComponent->StartLogic();
}

void ALLL_MonsterBaseAIController::StartDamagedHandle(UAnimMontage* Montage)
{
	if (Montage == MonsterDataAsset->DamagedAnimMontage)
	{
		StopLogic(TEXT("Monster Is Damaged"));
	}
}

void ALLL_MonsterBaseAIController::EndDamagedHandle(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MonsterDataAsset->DamagedAnimMontage && !Monster->IsKnockBacking())
	{
		if (Monster->CheckCharacterIsDead())
		{
			return;
		}

		BrainComponent->StartLogic();

		ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		SetPlayer(Player);
	}
}

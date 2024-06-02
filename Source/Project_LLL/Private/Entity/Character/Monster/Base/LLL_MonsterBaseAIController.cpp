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

void ALLL_MonsterBaseAIController::StartCoolDown()
{
	BlackboardComponent->SetValueAsBool(BBKEY_IS_COOL_DOWN, true);

	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	FTimerHandle CoolDownTimerHandle;
	GetWorldTimerManager().SetTimer(CoolDownTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		BlackboardComponent->SetValueAsBool(BBKEY_IS_COOL_DOWN, false);
	}), MonsterAttributeSet->GetAttackCoolDown(), false);
}

void ALLL_MonsterBaseAIController::StartDamagedHandle(UAnimMontage* Montage)
{
	if (Montage == MonsterDataAsset->DamagedAnimMontage)
	{
		BrainComponent->StopLogic("Monster Is Damaged");

		const FGameplayTagContainer WithOutTags = FGameplayTagContainer(TAG_GAS_ABILITY_NOT_CANCELABLE);
		Monster->GetAbilitySystemComponent()->CancelAbilities(nullptr, &WithOutTags);

		if (!IsValid(BlackboardComponent->GetValueAsObject(BBKEY_PLAYER)))
		{
			ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			if (IsValid(Player))
			{
				Player->SetAttacker(Monster);
			}
		}
	}
}

void ALLL_MonsterBaseAIController::EndDamagedHandle(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MonsterDataAsset->DamagedAnimMontage)
	{
		if (Monster->CheckCharacterIsDead())
		{
			return;
		}

		BrainComponent->StartLogic();

		if (!IsValid(BlackboardComponent->GetValueAsObject(BBKEY_PLAYER)))
		{
			ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			if (IsValid(Player))
			{
				BlackboardComponent->SetValueAsObject(BBKEY_PLAYER, Player);
			}
		}
	}
}

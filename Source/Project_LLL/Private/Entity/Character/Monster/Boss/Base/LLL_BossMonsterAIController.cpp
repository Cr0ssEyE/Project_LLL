// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/Base/LLL_BossMonsterAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Enumeration/LLL_BossMonsterPatternEnumHelper.h"
#include "Kismet/GameplayStatics.h"

void ALLL_BossMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, InPawn]{
		ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
		SetPlayer(Player);

		ALLL_BossMonster* BossMonster = CastChecked<ALLL_BossMonster>(InPawn);
		TArray<EBossMonsterPattern> CurrentHavePatterns = BossMonster->GetCurrentHavePatterns();
		if (CurrentHavePatterns.Num() > 0)
		{
			uint8 Pattern = static_cast<uint8>(CurrentHavePatterns[FMath::RandRange(0, CurrentHavePatterns.Num() - 1)]);
			BlackboardComponent->SetValueAsEnum(BBKEY_PATTERN, Pattern);
			BossMonster->SetChargeMontageKey(static_cast<EBossMonsterPattern>(Pattern));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("패턴이 존재하지 않습니다"))
		}
	}));

	StopLogic(TEXT("Stay"));

	FTimerHandle StartLogicTimerHandle;
	GetWorldTimerManager().SetTimer(StartLogicTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]{
		StartLogic();
	}), 3.0f, false);
}

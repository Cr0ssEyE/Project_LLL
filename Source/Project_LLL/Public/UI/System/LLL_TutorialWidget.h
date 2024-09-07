// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "LLL_TutorialWidget.generated.h"

class UTextBlock;

#define DASH_TUTORIAL_TEXT TEXT("대쉬(스페이스바)를 통해 건너편으로 이동하세요.")
#define FIGHT_TUTORIAL_TEXT TEXT("마우스 좌클릭 시 마우스 방향으로 공격합니다.")
#define REWARD_TUTORIAL_TEXT TEXT("보상을 획득하고 다음 방으로 이동하세요.")
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_TutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetDashTutorial() { TutorialTextBlock->SetText(FText::FromString(DASH_TUTORIAL_TEXT)); }
	FORCEINLINE void SetFightTutorial() { TutorialTextBlock->SetText(FText::FromString(FIGHT_TUTORIAL_TEXT)); }
	FORCEINLINE void SetRewardTutorial() { TutorialTextBlock->SetText(FText::FromString(REWARD_TUTORIAL_TEXT)); }
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> TutorialTextBlock;
};

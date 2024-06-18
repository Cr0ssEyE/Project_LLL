// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_MapDataAsset.h"
#include "LLL_TutorialMapDataAsset.generated.h"

class ULLL_TutorialWidget;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_TutorialMapDataAsset : public ULLL_MapDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "튜토리얼 룸 블루프린트")
	TSubclassOf<AActor> BPMap;

	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "공격 스테이지 스폰포인트")
	FVector AttackStageLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "최종 스테이지 스폰포인트")
	FVector FinalStageLocation;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "튜토리얼 UI")
	TSubclassOf<ULLL_TutorialWidget> TutorialWidgetClass;
};

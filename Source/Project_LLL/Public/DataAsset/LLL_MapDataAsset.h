// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_MapDataAsset.generated.h"

class ULevelSequence;
class ALLL_RewardObject;
class ALLL_MonsterSpawner;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MapDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "맵 블루프린트")
	TArray<TSubclassOf<AActor>> MapData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "최대 룸 개수")
	uint8 MaximumRoom;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "상점 룸 위치")
	uint8 StoreRoom;

	UPROPERTY(EditDefaultsOnly, Category = "Monster", DisplayName = "몬스터 스포너")
	TSubclassOf<ALLL_MonsterSpawner> MonsterSpawnerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Reward", DisplayName = "보상 오브젝트")
	TSubclassOf<ALLL_RewardObject> RewardObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fade", DisplayName = "페이드 인 시퀀스")
	TObjectPtr<ULevelSequence> FadeIn;

	UPROPERTY(EditDefaultsOnly, Category = "Fade", DisplayName = "페이드 아웃 시퀀스")
	TObjectPtr<ULevelSequence> FadeOut;
};

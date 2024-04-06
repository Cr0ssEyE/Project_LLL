// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_MapDataAsset.generated.h"

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
	
};

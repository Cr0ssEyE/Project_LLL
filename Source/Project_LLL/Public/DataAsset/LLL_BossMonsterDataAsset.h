// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "LLL_BossMonsterDataAsset.generated.h"

enum class EBossMonsterPattern : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_BossMonsterDataAsset : public ULLL_MonsterBaseDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Pattern", DisplayName = "시작할 때 가지는 패턴")
	TArray<EBossMonsterPattern> DefaultPatterns;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pattern", DisplayName = "체력이 해당 퍼센트에 도달할 때 추가 패턴")
	TMap<float, EBossMonsterPattern> AddPatterns;

	UPROPERTY(EditDefaultsOnly, Category = "Pattern", DisplayName = "체력이 해당 퍼센트에 도달할 때 제거 패턴")
	TMap<float, EBossMonsterPattern> RemovePatterns;
};

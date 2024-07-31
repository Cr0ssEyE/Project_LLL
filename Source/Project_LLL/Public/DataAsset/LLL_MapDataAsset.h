// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_MapDataAsset.generated.h"

class ALLL_GateObject;
class ULevelSequence;
class ALLL_RewardObject;
class ALLL_MonsterSpawner;
class ALLL_RewardGimmick;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MapDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "룸 블루프린트")
	TArray<TSubclassOf<AActor>> Rooms;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "상점 룸 블루프린트")
	TSubclassOf<AActor> Store;

	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "보스 룸 블루프린트")
	TSubclassOf<AActor> Boss;

	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "게이트 블루프린트")
	TSubclassOf<ALLL_GateObject> Gate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "최대 룸 개수")
	uint8 MaximumRoom;
	
	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "상점 룸 위치")
	uint8 StoreRoom;

	UPROPERTY(EditDefaultsOnly, Category = "Monster", DisplayName = "몬스터 스포너")
	TSubclassOf<ALLL_MonsterSpawner> MonsterSpawnerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Reward", DisplayName = "보상 오브젝트")
	TSubclassOf<ALLL_RewardObject> RewardObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence", DisplayName = "페이드 인 시퀀스")
	TObjectPtr<ULevelSequence> FadeIn;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence", DisplayName = "페이드 아웃 시퀀스")
	TObjectPtr<ULevelSequence> FadeOut;
	
	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "텔레포트 파티클")
	TObjectPtr<UNiagaraSystem> TeleportParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "텔레포트-페이드아웃 지연시간")
	float TeleportFadeOutDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "파티클 사이즈", meta=(DisplayPriority=1))
	FVector ParticleScale = FVector::OneVector;
};

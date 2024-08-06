// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_LobbyDataAsset.generated.h"

class UNiagaraSystem;
class ULevelSequence;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_LobbyDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Sequence", DisplayName = "로비 진입 시퀀스")
	TObjectPtr<ULevelSequence> LobbyEnterSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence", DisplayName = "던전 진입 시퀀스")
	TObjectPtr<ULevelSequence> DungeonEnterSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "텔레포트 파티클")
	TObjectPtr<UNiagaraSystem> TeleportParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "파티클 사이즈")
	FVector ParticleScale = FVector::OneVector;
	
	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "텔레포트-페이드아웃 지연시간")
	float TeleportFadeOutDelay;
	
};

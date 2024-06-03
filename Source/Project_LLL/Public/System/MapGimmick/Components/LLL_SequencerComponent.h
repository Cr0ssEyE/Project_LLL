// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LLL_SequencerComponent.generated.h"


class ULevelSequence;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_SequencerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	void PlayIntroSequence();

protected:
	UFUNCTION()
	void OnSequencerEndedCallBack(AActor* Actor, EEndPlayReason::Type EndPlayReason);
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "입장 인트로 재생 시퀀서")
	TObjectPtr<ULevelSequence> IntroSequencer;

	UPROPERTY(EditAnywhere, DisplayName = "레벨 이동 여부")
	uint8 bLoadToOtherLevel : 1;
};

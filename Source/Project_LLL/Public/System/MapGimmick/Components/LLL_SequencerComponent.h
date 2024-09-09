// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LLL_SequencerComponent.generated.h"


enum class ELevelSequenceType : uint8;
class ULevelSequence;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_SequencerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool CheckRoomEncounteredSequence() const { return bUseRoomEncounteredSequence; }
	FORCEINLINE bool CheckRoomAvoidSequence() const { return bUseRoomAvoidSequence; }
	FORCEINLINE ELevelSequenceType GetRoomEncounterSequenceID() const { return RoomEncounterSequenceID; }
	FORCEINLINE ELevelSequenceType GetRoomAvoidSequenceID() const { return RoomAvoidSequenceID; }
	
protected:
	UPROPERTY(EditAnywhere)
	uint8 bUseRoomEncounteredSequence : 1;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseRoomEncounteredSequence == true", EditConditionHides))
	ELevelSequenceType RoomEncounterSequenceID;
	
	UPROPERTY(EditAnywhere)
	uint8 bUseRoomAvoidSequence : 1;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseRoomAvoidSequence == true", EditConditionHides))
	ELevelSequenceType RoomAvoidSequenceID;
};

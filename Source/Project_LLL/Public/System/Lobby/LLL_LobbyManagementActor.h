// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_LobbyManagementActor.generated.h"

enum class ELobbyCustomPointType : uint8;
class ULLL_LobbyDataAsset;
class ALLL_WorldTreeObject;
class ALLL_DungeonEnteringObject;
class ALLL_LobbyCustomPoint;

UCLASS()
class PROJECT_LLL_API ALLL_LobbyManagementActor : public ALLL_SystemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_LobbyManagementActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	void SetUpDefaultLobby();
	void SetUpLobbyByGameProgress();
	void SetUpLobbyByEventProgress();
	void PlayerBeginEnter();
	
protected:
	UFUNCTION()
	void UpdateLobbyHousing(ELobbyCustomPointType PointType, uint32 PointID);
	
	UFUNCTION()
	void EnteringDungeon();

	UFUNCTION()
	void WorldTreeInteractionEvent();
	
	UFUNCTION()
	void PlayerEnterComplete(bool IsDrop = false);
	
protected:
	UPROPERTY()
	TObjectPtr<ULLL_LobbyDataAsset> LobbyDataAsset;

	UPROPERTY()
	TObjectPtr<UDataTable> LobbyDataTable;
	
protected:
	UPROPERTY()
	TArray<ALLL_LobbyCustomPoint*> CustomPoints;
	
	UPROPERTY()
	TObjectPtr<ALLL_DungeonEnteringObject> DungeonEnteringObject;

	UPROPERTY()
	TObjectPtr<ALLL_WorldTreeObject> WorldTreeObject;
	
protected:
	
};

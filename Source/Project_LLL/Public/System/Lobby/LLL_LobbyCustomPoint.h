// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LLL_LobbyCustomPoint.generated.h"


enum class ELobbyCustomPointType : uint8;

UCLASS()
class PROJECT_LLL_API ALLL_LobbyCustomPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ALLL_LobbyCustomPoint();

	FORCEINLINE ELobbyCustomPointType GetPointType() const { return CustomPointType; }
	FORCEINLINE uint32 GetPointID() const { return CustomPointID; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	ELobbyCustomPointType CustomPointType;

	UPROPERTY(EditAnywhere)
	uint32 CustomPointID;
	
};

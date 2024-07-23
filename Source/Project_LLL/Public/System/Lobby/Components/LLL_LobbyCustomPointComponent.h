// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LLL_LobbyCustomPointComponent.generated.h"


enum class ELobbyCustomPointType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_LobbyCustomPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULLL_LobbyCustomPointComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	ELobbyCustomPointType PointType;
	
};

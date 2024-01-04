// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LLL_ObjectBase.generated.h"

class UBoxComponent;

UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_ObjectBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_ObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> CollisionBox;
};

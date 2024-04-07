// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/LLL_InteractiveObject.h"
#include "LLL_GateObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_GateObject : public ALLL_InteractiveObject
{
	GENERATED_BODY()

public:
	ALLL_GateObject();

	FORCEINLINE void GateEnable() { IsGateEnabled = true; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void InteractiveEvent() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> GateMesh;

	UPROPERTY(EditDefaultsOnly)
	bool IsGateEnabled;

	void OpenGate();
};

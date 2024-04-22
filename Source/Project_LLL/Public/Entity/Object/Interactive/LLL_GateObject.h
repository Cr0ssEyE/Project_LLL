// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"
#include "LLL_GateObject.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStageDestroyDelegate);

UCLASS()
class PROJECT_LLL_API ALLL_GateObject : public ALLL_InteractiveObject
{
	GENERATED_BODY()

public:
	ALLL_GateObject();

	FORCEINLINE void GateEnable() { IsGateEnabled = true; }
	
	FOnStageDestroyDelegate StageDestroyDelegate;
	
protected:
	virtual void InteractiveEvent() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> GateMesh;

	UPROPERTY(EditDefaultsOnly)
	bool IsGateEnabled;

	void OpenGate();
};

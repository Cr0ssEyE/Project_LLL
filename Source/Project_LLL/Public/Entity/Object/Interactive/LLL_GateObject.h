// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "Entity/Object/Interactive/LLL_InteractiveObject.h"
#include "LLL_GateObject.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGateInteractionDelegate, FTestRewardDataTable*);

struct FTestRewardDataTable;
class ULLL_RewardDataTable;
UCLASS()
class PROJECT_LLL_API ALLL_GateObject : public ALLL_InteractiveObject
{
	GENERATED_BODY()

public:
	ALLL_GateObject();

	FORCEINLINE void GateEnable() { bIsGateEnabled = true; }
	FORCEINLINE FTestRewardDataTable* GetRewardData() const { return RewardData; }
	FORCEINLINE uint8 IsGateInteracted() const { return bIsGateInteracted; }
	
	FOnGateInteractionDelegate GateInteractionDelegate;

	void SetGateInformation(FTestRewardDataTable* Data);
	
protected:
	virtual void InteractiveEvent() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> GateMesh;

	UPROPERTY(EditDefaultsOnly)
	uint8 bIsGateEnabled : 1;
	
	FTestRewardDataTable* RewardData;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 bIsGateInteracted : 1;
	
	void OpenGate();

	void StartDestroy();
};

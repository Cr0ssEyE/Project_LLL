// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"
#include "LLL_GateObject.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGateInteractionDelegate, const FRewardDataTable*);

struct FRewardDataTable;
class ULLL_RewardDataTable;
class ULLL_GateDataAsset;
class ULLL_RewardObjectDataAsset;

UCLASS()
class PROJECT_LLL_API ALLL_GateObject : public ALLL_InteractiveObject
{
	GENERATED_BODY()

public:
	ALLL_GateObject();
	
	FORCEINLINE const FRewardDataTable* GetRewardData() const { return RewardData; }
	
	FOnGateInteractionDelegate GateInteractionDelegate;

	void SetGateInformation(const FRewardDataTable* Data);
	void SetActivate();
	
protected:
	virtual void InteractiveEvent() override;
	virtual void BeginPlay() override;
	
protected:
	void OpenGate();
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> GateMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TextureMeshComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> RewardTextureMesh;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const ULLL_RewardObjectDataAsset> RewardObjectDataAsset;

	UPROPERTY(EditDefaultsOnly)
	uint8 bIsGateEnabled : 1;
	
	const FRewardDataTable* RewardData;

	EAbilityType AbilityType;
};

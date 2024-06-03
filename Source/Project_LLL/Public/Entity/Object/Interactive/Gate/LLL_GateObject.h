// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"
#include "LLL_GateObject.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGateInteractionDelegate, const FRewardDataTable*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGateInteractionDelegate, FRewardDataTable*);
DECLARE_MULTICAST_DELEGATE(FOnFadeOutDelegate);

struct FRewardDataTable;
class ULLL_RewardDataTable;
class ULLL_GateDataAsset;
UCLASS()
class PROJECT_LLL_API ALLL_GateObject : public ALLL_InteractiveObject
{
	GENERATED_BODY()

public:
	ALLL_GateObject();
	
	FORCEINLINE const FRewardDataTable* GetRewardData() const { return RewardData; }
	
	FOnGateInteractionDelegate GateInteractionDelegate;
	FOnFadeOutDelegate FadeOutDelegate;

	void SetGateInformation(const FRewardDataTable* Data);
	void SetActivate();
	
protected:
	virtual void InteractiveEvent() override;
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> GateMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_GateDataAsset> GateDataAsset;

	UPROPERTY(EditDefaultsOnly)
	uint8 bIsGateEnabled : 1;
	
	const FRewardDataTable* RewardData;

	EAbilityType AbilityType;
	
	void OpenGate();

	void StartDestroy();

	UFUNCTION()
	void PlayerTeleport(UNiagaraComponent* InNiagaraComponent);
};

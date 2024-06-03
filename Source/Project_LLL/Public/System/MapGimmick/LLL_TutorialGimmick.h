// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Base/LLL_SystemBase.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "LLL_TutorialGimmick.generated.h"

class ALLL_GateObject;
class UStaticMeshComponent;
class ALLL_AbilityRewardObject;
class ULLL_GateSpawnPointComponent;
class ULLL_PlayerSpawnPointComponent;
class AActor;
class ULLL_MapDataAsset;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_TutorialGimmick : public ALLL_SystemBase
{
	GENERATED_BODY()

public:
	ALLL_TutorialGimmick();
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> StageBP;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> StageActor;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ALLL_GateObject> Gate;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ALLL_AbilityRewardObject> AbilityReward;
	
	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_PlayerSpawnPointComponent> PlayerSpawnPointComponent;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_GateSpawnPointComponent> GateSpawnPointComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<UNiagaraComponent> PlayerTeleportNiagara;

	UPROPERTY(VisibleAnywhere, Category = "stage")
	TObjectPtr<const ULLL_MapDataAsset> MapDataAsset;

	void OnInteractionGate(const FRewardDataTable* Data);

	UFUNCTION()
	void RewardDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void MonsterDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void LoadLevel(UNiagaraComponent* InNiagaraComponent);
};


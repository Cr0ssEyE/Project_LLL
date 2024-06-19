// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_TutorialGimmick.generated.h"

class ALLL_GateObject;
class UStaticMeshComponent;
class ALLL_RewardObject;
class ULLL_GateSpawnPointComponent;
class ULLL_PlayerSpawnPointComponent;
class AActor;
class UNiagaraComponent;
class ULLL_TutorialMapDataAsset;
class ULLL_TutorialWidget;
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
	TArray<TObjectPtr<AActor>> StageActors;
	
	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> Monsters;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ALLL_GateObject> Gate;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ALLL_RewardObject> AbilityReward;
	
	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_PlayerSpawnPointComponent> PlayerSpawnPointComponent;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_GateSpawnPointComponent> GateSpawnPointComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<UNiagaraComponent> PlayerTeleportNiagara;

	UPROPERTY(VisibleAnywhere, Category = "stage")
	TObjectPtr<const ULLL_TutorialMapDataAsset> TutorialDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "튜토리얼 UI")
	TSubclassOf<ULLL_TutorialWidget> TutorialWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_TutorialWidget> TutorialWidget;

	uint8 bIsActiveSkill : 1;

	uint8 bIsActiveDash : 1;
	
	void OnInteractionGate();

	UFUNCTION()
	void RewardDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void MonsterDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void BeginOverlapAttackTutorial(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void FinalMapSpawn(AActor* DestroyedActor);

	UFUNCTION()
	void FinalMonsterSpawn(AActor* DestroyedActor);

	UFUNCTION()
	void LoadLevel(UNiagaraComponent* InNiagaraComponent);

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "플레이어 최대 체력 증가 이펙트")
	TSubclassOf<UGameplayEffect> ChargeSkillGaugeEffect;
};

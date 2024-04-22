// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "GameFramework/Actor.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_MapGimmick.generated.h"

class UBoxComponent;
class ULLL_MapDataAsset;
class ALLL_GateObject;
class ALLL_RewardObject;
class ALLL_MonsterSpawner;
class ULevelSequencePlayer;
class ULevelSequence;
class ALevelSequenceActor;
class ULLL_ShoppingMapComponent;
class ULLL_RewardDataTable;
class ALLL_RewardGimmick;
class ULLL_PlayerSpawnPointComponent;

DECLARE_DELEGATE(FOnStageChangedDelegate);

USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageChangedDelegateWrapper() { }
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
	FOnStageChangedDelegate StageDelegate;
};

UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT,
	BURST
};

UCLASS()
class PROJECT_LLL_API ALLL_MapGimmick : public ALLL_SystemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALLL_MapGimmick();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> RootBox;
	
	// Stage Section
protected:
	UPROPERTY(VisibleAnywhere, Category = "stage")
	TObjectPtr<ULLL_MapDataAsset> MapDataAsset;
	
	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> Stage;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> StageTrigger;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> StageActor;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> StageChildActors;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_ShoppingMapComponent> ShoppingMapComponent;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_PlayerSpawnPointComponent> PlayerSpawnPointComponent;

	uint32 Seed;

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CreateMap();

	UFUNCTION()
	void RandomMap();

	UFUNCTION()
	void ChangeMap(AActor* DestroyedActor);

private:
	UPROPERTY()
	uint8 GateIndex;

// Gate Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALLL_GateObject> GateClass;

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<ALLL_GateObject>> Gates;
	
	UFUNCTION()
	void AllGatesDestroy();
	
	void OnInteractionGate(FTestRewardDataTable* Data);
	
	void EnableAllGates();

// State Section
protected:
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	void SetState(EStageState InNewState);

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;

	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

// Fight Section
protected:
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALLL_MonsterSpawner> MonsterSpawnerClass;
	
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ALLL_MonsterSpawner> MonsterSpawner;
	
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);
	
	void OnOpponentSpawn();

// Reward Section
protected:
	UPROPERTY(EditAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALLL_RewardObject> RewardObjectClass;

	UPROPERTY(EditAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ALLL_RewardGimmick> RewardGimmick;
	
	FTestRewardDataTable* RewardData;

	UFUNCTION()
	void RewardDestroyed(AActor* DestroyedActor);

	void RewardSpawn();

//Sequence Section
protected:
	UPROPERTY(EditDefaultsOnly, Category = Sequence)
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY(EditDefaultsOnly, Category = Sequence)
	TObjectPtr<ULevelSequence> FadeInSequence;

	UPROPERTY(EditDefaultsOnly, Category = Sequence)
	TObjectPtr<ULevelSequence> FadeOutSequence;

	UPROPERTY(EditDefaultsOnly, Category = Sequence)
	TObjectPtr<ALevelSequenceActor> LevelSequenceActor;
};

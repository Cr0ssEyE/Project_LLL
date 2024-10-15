// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "DataAsset/LLL_MapDataAsset.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "GameFramework/Actor.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_MapGimmick.generated.h"

class ULLL_SequencerComponent;
enum class EStageState : uint8;
class UBoxComponent;
class ALLL_GateObject;
class ALLL_AbilityRewardObject;
class ALLL_MonsterSpawner;
class ULevelSequencePlayer;
class ULevelSequence;
class ALevelSequenceActor;
class ULLL_ShoppingMapComponent;
class ULLL_RewardDataTable;
class ALLL_RewardGimmick;
class ULLL_PlayerSpawnPointComponent;
class UNiagaraComponent;

DECLARE_DELEGATE(FOnStageChangedDelegate);

USTRUCT(BlueprintType)
struct FStageInfoData
{
	GENERATED_BODY()
	
public:
	FStageInfoData():
	Seed(UINT32_MAX),
	RoomNumber(UINT32_MAX),
	PlayerLocation(FVector::Zero()),
	RewardPosition(FVector::Zero()),
	LastStageState(EStageState::READY),
	bIsLoadedFromSave(false)
	{
		
	}

public:
	UPROPERTY()
	uint32 Seed;
	
	UPROPERTY()
	uint32 RoomNumber;

	UPROPERTY()
	TArray<uint8> GatesRewardID;

	UPROPERTY()
	FVector PlayerLocation;

	UPROPERTY()
	FVector RewardPosition;

	UPROPERTY()
	EStageState LastStageState;
	
	UPROPERTY()
	TArray<int32> SpawnedAbilityDataIDArray;

	UPROPERTY()
	uint32 bIsLoadedFromSave : 1;
};

USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageChangedDelegateWrapper() { }
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
	FOnStageChangedDelegate StageDelegate;
};

UCLASS()
class PROJECT_LLL_API ALLL_MapGimmick : public ALLL_SystemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALLL_MapGimmick();

public:
	FStageInfoData MakeStageInfoData();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	void SetupLevel();

public:
	FORCEINLINE ALLL_MonsterSpawner* GetMonsterSpawner() const { return MonsterSpawner; }
	FORCEINLINE bool CheckShoppingRoom() const { return ShoppingMapComponent != nullptr; }
	FORCEINLINE ULLL_ShoppingMapComponent* GetShoppingMapComponent() const { return ShoppingMapComponent; }
	FORCEINLINE EStageState GetStageState() const { return CurrentState; }
	FORCEINLINE ALLL_RewardGimmick* GetRewardGimmick() const { return RewardGimmick; }
	FORCEINLINE FVector GetRewardPosition() const { return RewardObjectPosition; }
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "stage")
	TObjectPtr<const ULLL_MapDataAsset> MapDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> RootBox;
	
	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> RoomClass;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RoomTrigger;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> RoomActor;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> RoomChildActors;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_ShoppingMapComponent> ShoppingMapComponent;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_PlayerSpawnPointComponent> PlayerSpawnPointComponent;

	UPROPERTY(VisibleAnywhere, Category = "stage", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULLL_SequencerComponent> RoomSequencerPlayComponent;
	
	uint32 Seed;

protected:
	UFUNCTION()
	void LoadLastSessionMap(FStageInfoData StageInfoData);
	
	UFUNCTION()
	void CreateMap();

	UFUNCTION()
	void RandomMap();

	UFUNCTION()
	void ChangeMap(AActor* DestroyedActor);

private:
	UPROPERTY(VisibleAnywhere)
	uint32 CurrentRoomNumber;

// Gate Section
protected:
	UPROPERTY(VisibleAnywhere, Category = "Gate", Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<ALLL_GateObject>> Gates;
	
	UPROPERTY(EditAnywhere, Category = "Gate", Meta = (AllowPrivateAccess = "true"))
	FName LevelName;

	UFUNCTION()
	void ChangeLevel();

	uint8 bIsNextGateInteracted : 1;
	
protected:
	UFUNCTION()
	void AllGatesDestroy();
	
	void OnInteractionGate(const FRewardDataTable* Data);
	void EnableAllGates();

	UFUNCTION()
	void SetupGateData();
	
// State Section
protected:
	UPROPERTY(EditAnywhere, Category = "Stage", Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;

	uint8 bIsFirstLoad : 1;

	uint8 bIsLoadedFromSave : 1;
	
protected:
	void SetState(EStageState InNewState);
	
	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

// Fight Section
protected:
	UPROPERTY(EditAnywhere, Category = "Fight", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALLL_MonsterSpawner> MonsterSpawnerClass;
	
	UPROPERTY(EditAnywhere, Category = "Fight", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ALLL_MonsterSpawner> MonsterSpawner;
	
	UPROPERTY(EditAnywhere, Category = "Fight", Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	UFUNCTION()
	void OnOpponentSpawn();
	
	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);

// Reward Section
protected:
	UPROPERTY(EditAnywhere, Category = "Reward", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALLL_AbilityRewardObject> RewardObjectClass;

	UPROPERTY(EditAnywhere, Category = "Reward", Meta = (AllowPrivateAccess = "true"))
	FVector RewardObjectPosition;
	
	UPROPERTY(EditAnywhere, Category = "Reward", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ALLL_RewardGimmick> RewardGimmick;
	
	const FRewardDataTable* RewardData;

	UFUNCTION()
	void RewardDestroyed(AActor* DestroyedActor);

	void RewardSpawn();

	UFUNCTION()
	void SetRewardWidget();

//Sequence Section
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<ULevelSequencePlayer> FadeInSequencePlayer;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<ULevelSequencePlayer> FadeOutSequencePlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<ULevelSequence> FadeInSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<ULevelSequence> FadeOutSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<ALevelSequenceActor> FadeInSequenceActor;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<ALevelSequenceActor> FadeOutSequenceActor;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence")
	TObjectPtr<UNiagaraComponent> PlayerTeleportNiagara;

protected:
	UFUNCTION()
	void FadeIn();

	UFUNCTION()
	void FadeOut();

	UFUNCTION()
	void PlayerTeleport();
	
	UFUNCTION()
	void PlayerSetHidden();

	UFUNCTION()
	void PlayEncounterSequence();
};

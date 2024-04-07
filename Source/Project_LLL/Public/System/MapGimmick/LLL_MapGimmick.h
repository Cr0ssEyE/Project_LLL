// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LLL_MapGimmick.generated.h"

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
class PROJECT_LLL_API ALLL_MapGimmick : public AActor
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
	TObjectPtr<class UBoxComponent> RootBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<class UBoxComponent> ReadyBox;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<class UBoxComponent> FightBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<class UBoxComponent> RewardBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<class UBoxComponent> NextBox;
	// Stage Section
protected:
	UPROPERTY(VisibleAnywhere, Category = stage)
	TObjectPtr<class ULLL_MapDataAsset> MapDataAsset;
	
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> Stage;

	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnReadyBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnFightBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRewardBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnNextBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CreateMap();

	UFUNCTION()
	void RandomMap();

private:
	UPROPERTY()
	uint8 GateIndex;

// Gate Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ALLL_GateObject> GateClass;

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class ALLL_GateObject>> Gates;

	TMap<FName, FVector> GateLocations;

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
	TSubclassOf<class ALLL_MonsterSpawner> MonsterSpawnerClass;
	
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ALLL_MonsterSpawner> MonsterSpawner;
	
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);
	
	void OnOpponentSpawn();

// Reward Section
protected:
	UPROPERTY(EditAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ALLL_RewardObject> RewardObjectClass;

	UFUNCTION()
	void RewardDestroyed(AActor* DestroyedActor);

	void RewardSpawn();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_GameInstance.h"
#include "Engine/GameInstance.h"
#include "ProtoGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMonsterToggleAIDelegate, bool, value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectTrapActivateDelegate);

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT_LLL_API UProtoGameInstance : public ULLL_GameInstance
{
	GENERATED_BODY()
	
public:
	UProtoGameInstance();

	// 디버그용 델리게이트
public:
	UPROPERTY(BlueprintCallable)
	FMonsterToggleAIDelegate MonsterToggleAIDelegate;

	UPROPERTY(BlueprintCallable)
	FObjectTrapActivateDelegate ObjectTrapActivateDelegate;
	
	// 플레이어 디버그 변수 Getter / Setter
public:
	FORCEINLINE void SetPlayerMovementDebug(bool value) { bPlayerMovementDebug = value; }
	FORCEINLINE void SetPlayerDashDebug(bool value) { bPlayerDashDebug = value; }
	FORCEINLINE void SetPlayerSkillDebug(bool value) { bPlayerSkillDebug = value; }
	FORCEINLINE void SetPlayerWireActionDebug(bool value) { bPlayerWireActionDebug = value; }
	FORCEINLINE void SetPlayerHitCheckDebug(bool value) { bPlayerHitCheckDebug = value; }
	FORCEINLINE void SetPlayerAttackDebug(bool value) { bPlayerAttackDebug = value; }
	FORCEINLINE void SetPlayerCollisionDebug(bool value) { bPlayerCollisionDebug = value; }
	FORCEINLINE void SetPlayerInteractionDebug(bool value) { bPlayerInteractionDebug = value; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckPlayerMovementDebug() const { return bPlayerMovementDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckPlayerDashDebug() const { return bPlayerDashDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckPlayerSkillDebug() const { return bPlayerSkillDebug; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckPlayerWireActionDebug() const { return bPlayerWireActionDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckPlayerHitDebug() const { return bPlayerHitCheckDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckPlayerAttackDebug() const { return bPlayerAttackDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckPlayerCollisionDebug() const { return bPlayerCollisionDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckPlayerInteractionDebug() const { return bPlayerInteractionDebug; }
	
	// 몬스터 디버그 변수 Getter / Setter
public:
	FORCEINLINE void SetMonsterSpawnDataDebug(bool value) { bMonsterSpawnDataDebug = value; }
	FORCEINLINE void SetMonsterToggleAIDebug(bool value) { bMonsterToggleAIDebug = value; }
	FORCEINLINE void SetMonsterHitCheckDebug(bool value) { bMonsterHitCheckDebug = value; }
	FORCEINLINE void SetMonsterAttackDebug(bool value) { bMonsterAttackDebug = value; }
	FORCEINLINE void SetMonsterCollisionDebug(bool value) { bMonsterCollisionDebug = value; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckMonsterSpawnDataDebug() const { return bMonsterSpawnDataDebug; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetMonsterToggleAIDebug() const { return bMonsterToggleAIDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void BroadcastToggleAI() const { MonsterToggleAIDelegate.Broadcast(bMonsterToggleAIDebug); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckMonsterHitCheckDebug() const { return bMonsterHitCheckDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckMonsterAttackDebug() const { return bMonsterAttackDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckMonsterCollisionDebug() const { return bMonsterCollisionDebug; }
	
	// 오브젝트 디버그 변수 Getter / Setter
public:
	FORCEINLINE void SetObjectGenerateDebug(bool value) { bObjectGenerateDebug = value; }
	FORCEINLINE void SetObjectActivateDebug(bool value) { bObjectActivateDebug = value; }
	FORCEINLINE void SetObjectDestroyDebug(bool value) { bObjectDestroyDebug = value; }
	FORCEINLINE void SetObjectHitCheckDebug(bool value) { bObjectHitCheckDebug = value; }
	FORCEINLINE void SetObjectCollisionDebug(bool value) { bObjectCollisionDebug = value; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void BroadcastObjectTrapActivate() const { ObjectTrapActivateDelegate.Broadcast(); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckObjectGenerateDebug() const { return bMonsterHitCheckDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckObjectActivateDebug() const { return bObjectActivateDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckObjectDestroyDebug() const { return bObjectDestroyDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckObjectHitCheckDebug() const { return bObjectHitCheckDebug; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckObjectCollisionDebug() const { return bObjectCollisionDebug; }


	// 플레이어 디버그 변수
protected:
	uint8 bPlayerMovementDebug : 1;

	uint8 bPlayerDashDebug : 1;

	uint8 bPlayerWireActionDebug : 1;
	
	uint8 bPlayerSkillDebug : 1;

	uint8 bPlayerHitCheckDebug : 1;

	uint8 bPlayerAttackDebug : 1;

	uint8 bPlayerCollisionDebug : 1;

	uint8 bPlayerInteractionDebug : 1;
	
	// 몬스터 디버그 변수
protected:
	uint8 bMonsterSpawnDataDebug : 1;
	
	uint8 bMonsterToggleAIDebug : 1;
	
	uint8 bMonsterHitCheckDebug : 1;

	uint8 bMonsterAttackDebug : 1;

	uint8 bMonsterCollisionDebug : 1;

	// 오브젝트 디버그 변수
protected:
	uint8 bObjectGenerateDebug : 1;

	uint8 bObjectActivateDebug : 1;

	uint8 bObjectDestroyDebug : 1;

	uint8 bObjectHitCheckDebug : 1;

	uint8 bObjectCollisionDebug : 1;
};

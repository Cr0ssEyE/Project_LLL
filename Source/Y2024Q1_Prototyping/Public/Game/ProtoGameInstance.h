// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProtoGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FMonsterDisableAIDelegate)
DECLARE_MULTICAST_DELEGATE(FObjectTrapActivateDelegate)

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API UProtoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UProtoGameInstance();

	// 디버그용 델리게이트
public:
	FMonsterDisableAIDelegate MonsterDisableAIDelegate;
	
	FObjectTrapActivateDelegate ObjectTrapActivateDelegate;
	
	// 플레이어 디버그 변수 Getter / Setter
public:
	FORCEINLINE void SetPlayerMovementDebug(bool value) { bPlayerMovementDebug = value; }
	FORCEINLINE void SetPlayerDashDebug(bool value) { bPlayerDashDebug = value; }
	FORCEINLINE void SetPlayerSkillDebug(bool value) { bPlayerSkillDebug = value; }
	FORCEINLINE void SetPlayerHitCheckDebug(bool value) { bPlayerHitCheckDebug = value; }
	FORCEINLINE void SetPlayerAttackDebug(bool value) { bPlayerAttackDebug = value; }
	FORCEINLINE void SetPlayerCollisionDebug(bool value) { bPlayerCollisionDebug = value; }
	
	FORCEINLINE bool CheckPlayerMovementDebug() const { return bPlayerMovementDebug; }
	FORCEINLINE bool CheckPlayerDashDebug() const { return bPlayerDashDebug; }
	FORCEINLINE bool CheckPlayerSkillDebug() const { return bPlayerSkillDebug; }
	FORCEINLINE bool CheckPlayerHitDebug() const { return bPlayerHitCheckDebug; }
	FORCEINLINE bool CheckPlayerAttackDebug() const { return bPlayerAttackDebug; }
	FORCEINLINE bool CheckPlayerCollisionDebug() const { return bPlayerCollisionDebug; }
	
	// 몬스터 디버그 변수 Getter / Setter
public:
	FORCEINLINE void SetMonsterHitCheckDebug(bool value) { bMonsterHitCheckDebug = value; }
	FORCEINLINE void SetMonsterAttackDebug(bool value) { bMonsterAttackDebug = value; }
	FORCEINLINE void SetMonsterCollisionDebug(bool value) { bMonsterCollisionDebug = value; }

	FORCEINLINE void BroadcastDisableAI() const { MonsterDisableAIDelegate.Broadcast(); }
	FORCEINLINE bool CheckMonsterHitCheckDebug() const { return bMonsterHitCheckDebug; }
	FORCEINLINE bool CheckMonsterAttackDebug() const { return bMonsterAttackDebug; }
	FORCEINLINE bool CheckMonsterCollisionDebug() const { return bMonsterCollisionDebug; }
	
	// 오브젝트 디버그 변수 Getter / Setter
public:
	FORCEINLINE void SetObjectGenerateDebug(bool value) { bObjectGenerateDebug = value; }
	FORCEINLINE void SetObjectActivateDebug(bool value) { bObjectActivateDebug = value; }
	FORCEINLINE void SetObjectDestroyDebug(bool value) { bObjectDestroyDebug = value; }
	FORCEINLINE void SetObjectHitCheckDebug(bool value) { bObjectHitCheckDebug = value; }
	FORCEINLINE void SetObjectCollisionDebug(bool value) { bObjectCollisionDebug = value; }

	FORCEINLINE void BroadcastObjectTrapActivate() const { ObjectTrapActivateDelegate.Broadcast(); }
	FORCEINLINE bool CheckObjectGenerateDebug() const { return bMonsterHitCheckDebug; }
	FORCEINLINE bool CheckObjectActivateDebug() const { return bObjectActivateDebug; }
	FORCEINLINE bool CheckObjectDestroyDebug() const { return bObjectDestroyDebug; }
	FORCEINLINE bool CheckObjectHitCheckDebug() const { return bObjectHitCheckDebug; }
	FORCEINLINE bool CheckObjectCollisionDebug() const { return bObjectCollisionDebug; }
	
	// 플레이어 디버그 변수
protected:
	uint8 bPlayerMovementDebug : 1;

	uint8 bPlayerDashDebug : 1;

	uint8 bPlayerSkillDebug : 1;

	uint8 bPlayerHitCheckDebug : 1;

	uint8 bPlayerAttackDebug : 1;

	uint8 bPlayerCollisionDebug : 1;
	
	// 몬스터 디버그 변수
protected:
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_GameInstance.generated.h"

class ILLL_PlayerDependencyActorInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerActorAssignedDelegate, AActor*, AssignedActor);

/**
 * 
 */

UCLASS()
class PROJECT_LLL_API ULLL_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ULLL_GameInstance();

	// 플레이어 관련 객체 저장 및 관리용
public:
	FORCEINLINE TArray<TSoftObjectPtr<AActor>>& GetPlayerDependencyActors() { return PlayerDependencyActors; }
	
	// 플레이어 관련 객체들은 월드 스폰시 불릿 타임 영향을 받지 않도록 인터페이스 상속 및 등록 필요.
	void AssignPlayerDependencyActors(AActor* Actor);
	
	FPlayerActorAssignedDelegate PlayerActorAssignedDelegate;
	
protected:
	UPROPERTY()
	TArray<TSoftObjectPtr<AActor>> PlayerDependencyActors;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> PlayerMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> ObjectMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> MonsterMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> InterfaceMPC;
	
};

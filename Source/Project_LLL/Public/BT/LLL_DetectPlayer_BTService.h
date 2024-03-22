// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "LLL_DetectPlayer_BTService.generated.h"

class ALLL_MonsterBase;
class ALLL_PlayerBase;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_DetectPlayer_BTService : public UBTService
{
	GENERATED_BODY()

public:
	ULLL_DetectPlayer_BTService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void DetectPlayer(UBehaviorTreeComponent& OwnerComp, ALLL_MonsterBase* MonsterBase, float FieldOfView) const;
	static bool IsPlayerInFieldOfView(const ALLL_MonsterBase* MonsterBase, const ALLL_PlayerBase* PlayerBase, float FieldOfView);
	bool LineOfSightToPlayer(ALLL_MonsterBase* MonsterBase, ALLL_PlayerBase* PlayerBase) const;

	UPROPERTY()
	uint8 Detected : 1;
};

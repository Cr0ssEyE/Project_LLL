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
class Y2024Q1_PROTOTYPING_API ULLL_DetectPlayer_BTService : public UBTService
{
	GENERATED_BODY()

public:
	ULLL_DetectPlayer_BTService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	static bool IsPlayerInFieldOfView(const ALLL_MonsterBase* MonsterBase, const ALLL_PlayerBase* PlayerBase, float FieldOfView);
	bool LineOfSightToPlayer(ALLL_MonsterBase* MonsterBase, ALLL_PlayerBase* PlayerBase) const;
};

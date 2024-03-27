// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "LLL_CheckRange_BTService.generated.h"

class ALLL_PlayerBase;
class ALLL_MonsterBase;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CheckRange_BTService : public UBTService
{
	GENERATED_BODY()

public:
	ULLL_CheckRange_BTService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

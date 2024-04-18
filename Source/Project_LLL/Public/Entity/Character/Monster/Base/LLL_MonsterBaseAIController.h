// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LLL_MonsterBaseAIController.generated.h"

class UAISenseConfig_Sight;
class ALLL_BaseCharacter;
class ALLL_MonsterBase;
class ULLL_MonsterBaseDataAsset;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MonsterBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALLL_MonsterBaseAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	void AISenseInit() const;
	void MontageDelegateInit();

	UFUNCTION()
	void StartDamagedHandle(UAnimMontage* Montage);

	UFUNCTION()
	void EndDamagedHandle(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ALLL_MonsterBase> Monster;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;
};

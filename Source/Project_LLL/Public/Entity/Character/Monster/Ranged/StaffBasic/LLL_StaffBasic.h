// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_StaffBasicDataAsset.h"
#include "Entity/Character/Monster/Ranged/Base/LLL_RangedMonster.h"
#include "LLL_StaffBasic.generated.h"

class ULLL_StaffBasicAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_StaffBasic : public ALLL_RangedMonster
{
	GENERATED_BODY()

public:
	ALLL_StaffBasic();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_StaffBasicDataAsset> StaffBasicDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_StaffBasicAttributeSet> StaffBasicAttributeSet;

	// Todo : 어빌리티 작업이 끝난 후 커브 데이터로 옮기기
	TSubclassOf<UGameplayEffect> InitEffect;
};

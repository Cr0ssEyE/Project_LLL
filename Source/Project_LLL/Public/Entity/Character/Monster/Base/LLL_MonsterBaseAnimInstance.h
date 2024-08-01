// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Base/LLL_BaseCharacterAnimInstance.h"
#include "LLL_MonsterBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterBaseAnimInstance : public ULLL_BaseCharacterAnimInstance
{
	GENERATED_BODY()

public:
	ULLL_MonsterBaseAnimInstance();

	FORCEINLINE void SetSnapped(bool IsSnapped) { bIsSnapped = IsSnapped; }
	FORCEINLINE bool IsSnapped() const { return bIsSnapped; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	uint8 bIsSnapped : 1;
};

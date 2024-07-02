// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseASC.h"
#include "LLL_PlayerASC.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_PlayerASC : public ULLL_BaseASC
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULLL_PlayerASC();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};

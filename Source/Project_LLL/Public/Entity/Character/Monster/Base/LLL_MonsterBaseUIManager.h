// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Base/LLL_BaseCharacterUIManager.h"
#include "LLL_MonsterBaseUIManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_MonsterBaseUIManager : public ULLL_BaseCharacterUIManager
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULLL_MonsterBaseUIManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

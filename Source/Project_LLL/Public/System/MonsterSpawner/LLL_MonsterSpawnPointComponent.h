// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "DataAsset/LLL_MonsterSpawnerDataAsset.h"
#include "LLL_MonsterSpawnPointComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_LLL_API ULLL_MonsterSpawnPointComponent : public UBillboardComponent
{
	GENERATED_BODY()

public:
	ULLL_MonsterSpawnPointComponent();
	
	FORCEINLINE int32 GetNum() const { return Num; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int32 Num;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterSpawnerDataAsset> MonsterSpawnerDataAsset;
};

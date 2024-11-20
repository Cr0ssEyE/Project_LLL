// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/Global/LLL_GlobalNiagaraDataAsset.h"
#include "Entity/Object/Base/LLL_BaseObject.h"
#include "GameFramework/Actor.h"
#include "LLL_BreakableObjectBase.generated.h"

class UCapsuleComponent;

UCLASS()
class PROJECT_LLL_API ALLL_BreakableObjectBase : public ALLL_BaseObject
{
	GENERATED_BODY()
	
public:
	ALLL_BreakableObjectBase();

protected:
	virtual void BeginPlay() override;

public:
	void ReceivePlayerAttackOrKnockBackedMonster();
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_GlobalNiagaraDataAsset> GlobalNiagaraDataAsset;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	int32 Crack;
	
	UPROPERTY(EditDefaultsOnly)
	float StunRadius;
	
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UTexture> DestroyedTexture;
};

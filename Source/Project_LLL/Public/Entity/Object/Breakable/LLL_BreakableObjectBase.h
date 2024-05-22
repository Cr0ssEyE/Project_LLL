// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Entity/Object/Base/LLL_BaseObject.h"
#include "Interface/LLL_DropGoldInterface.h"
#include "GAS/Attribute/DropGold/LLL_DropGoldAttributeSet.h"
#include "GameFramework/Actor.h"
#include "LLL_BreakableObjectBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDropGoldDelegate)

UCLASS()
class PROJECT_LLL_API ALLL_BreakableObjectBase : public ALLL_BaseObject, public ILLL_DropGoldInterface
{
	GENERATED_BODY()
	
public:
	ALLL_BreakableObjectBase();

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void DropGold(const FGameplayTag tag, int32 data) override;
	FOnDropGoldDelegate GoldDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UCapsuleComponent> HitCollision;

//GAS Part
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "어트리뷰트 초기화 이펙트")
	TSubclassOf<UGameplayEffect> InitEffect;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_DropGoldAttributeSet> DropGoldAttributeSet;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Interface/LLL_PlayerDependencyInterface.h"
#include "LLL_BaseASC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataDelegate, const FGameplayAbilityTargetDataHandle&, Handle);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_BaseASC : public UAbilitySystemComponent, public ILLL_PlayerDependencyInterface
{
	GENERATED_BODY()

public:
	ULLL_BaseASC();

	FTargetDataDelegate TargetDataDelegate;

	void ReceiveTargetData(const UGameplayAbility* OwnerAbility, const FGameplayAbilityTargetDataHandle& TargetDataHandle) const;
	
protected:
	
};

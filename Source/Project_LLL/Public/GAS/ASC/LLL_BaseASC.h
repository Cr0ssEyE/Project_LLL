// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Interface/LLL_PlayerDependencyInterface.h"
#include "LLL_BaseASC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataDelegate, const FGameplayAbilityTargetDataHandle&, Handle);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_BaseASC : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	ULLL_BaseASC();

	virtual void BeginPlay() override;

	// 델리게이트
public:
	FTargetDataDelegate TargetDataDelegate;

	void ReceiveTargetData(const UGameplayAbility* OwnerAbility, const FGameplayAbilityTargetDataHandle& TargetDataHandle) const;

	// 외부 호출 기능
public:
	FORCEINLINE FActiveGameplayEffect* GetEditableActiveGameplayEffect(const FActiveGameplayEffectHandle EffectHandle) { return ActiveGameplayEffects.GetActiveGameplayEffect(EffectHandle); }
	
	void ReduceCoolDownEffectDuration(const FActiveGameplayEffectHandle& ActiveGameplayEffectHandle, float ReduceAmount);
	
};

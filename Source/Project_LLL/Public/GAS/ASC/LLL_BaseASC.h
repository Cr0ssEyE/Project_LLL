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
	
	FTargetDataDelegate TargetDataDelegate;

	void ReceiveTargetData(const UGameplayAbility* OwnerAbility, const FGameplayAbilityTargetDataHandle& TargetDataHandle) const;

protected:
	// TODO: 플레이어랑 몬스터 ASC 분리하기
	virtual void OnFallableTagAdded(const FGameplayTag Tag, int32 count);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_KnockBackCollisionCheck.generated.h"

class ALLL_BaseCharacter;
class ULLL_ExtendedGameplayEffect;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_KnockBackCollisionCheck : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_KnockBackCollisionCheck();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "넉백된 대상에게 적용되는 이펙트")
	TSubclassOf<UGameplayEffect> CollideCauserApplyEffect;
	
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "충돌 대상에게 적용되는 이펙트")
	TSubclassOf<UGameplayEffect> CollideTargetApplyEffect;
	
protected:
	UFUNCTION()
	void OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
	UFUNCTION()
	void OnOtherActorCollidedCallBack(AActor* HitActor, AActor* OtherActor);

	UPROPERTY()
	TArray<TObjectPtr<ALLL_BaseCharacter>> KnockBackedCharacters;
};

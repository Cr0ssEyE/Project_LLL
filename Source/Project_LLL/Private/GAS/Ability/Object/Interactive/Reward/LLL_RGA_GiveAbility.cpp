// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Object/Interactive/Reward/LLL_RGA_GiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

void ULLL_RGA_GiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	const FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Player);
	BP_ApplyGameplayEffectToTarget(TargetDataHandle, RewardAbilityEffect);

	UE_LOG(LogTemp, Log, TEXT("능력 부여"))*/

	ULLL_AbilityManageSubSystem* AbilityManageSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<ULLL_AbilityManageSubSystem>();
	if (IsValid(AbilityManageSubSystem))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Silver, FString::Printf(TEXT("서브 시스템 생성 확인")));
		// AsyncLoadEffectDelegate.AddDynamic(this, &ALLL_PlayerBase::DelegateReceiveTest);
		FAsyncLoadEffectDelegate Delegate;
		Delegate.AddDynamic(this, &ALLL_PlayerBase::DelegateReceiveTest);
		AbilityManageSubSystem->ASyncLoadEffectsByTag(Delegate, EEffectOwnerType::Player, FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Tests.Dummy"))), true);
	}
}

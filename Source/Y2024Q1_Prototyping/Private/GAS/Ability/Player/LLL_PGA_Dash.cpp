// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_Dash.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_MonatgeSectionName.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

ULLL_PGA_Dash::ULLL_PGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentDashCount = 0;
}

void ULLL_PGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// ASC에 등록된 어트리뷰트 가져오고 GA에서 필요한 어트리뷰트 저장하기
	ALLL_PlayerBase * PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));
	if(IsValid(PlayerCharacter) && IsValid(PlayerAttributeSet))
	{
		DashSpeed = PlayerAttributeSet->GetDashSpeed();
		MaxDashCount = PlayerAttributeSet->GetMaxDashCount();
	}
	bIsInputPressed = true;
	DashActionEvent();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 발동 %f"), PlayerAttributeSet->GetCurrentHealth()));
}

void ULLL_PGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerBase * PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	}
	CurrentDashCount = 0;
	DashInputDelayHandle.Invalidate();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_Dash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	if(DashInputDelayHandle.IsValid())
	{
		bIsInputPressed = true;
		DashActionEvent();
	}
}

void ULLL_PGA_Dash::DashActionEvent()
{
	DashInputDelayHandle.Invalidate();
	ALLL_PlayerBase * PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));
	
	if(IsValid(PlayerCharacter) && IsValid(PlayerAttributeSet) && bIsInputPressed && CurrentDashCount < MaxDashCount)
	{
		CurrentDashCount++;
		PlayerCharacter->LaunchCharacter(PlayerCharacter->GetActorForwardVector() * (DashSpeed * 1000.f), true, true);
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
		PlayerCharacter->StopAnimMontage(DashAnimMontage);
		PlayerCharacter->PlayAnimMontage(DashAnimMontage);
		StartDashInputDelay();
		bIsInputPressed = false;
	}
}

void ULLL_PGA_Dash::StartDashInputDelay()
{
	GetWorld()->GetTimerManager().SetTimer(DashInputDelayHandle, this, &ULLL_PGA_Dash::EndDashInputDelay, DashInputCheckTime, false);
}

void ULLL_PGA_Dash::EndDashInputDelay()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 어빌리티 종료")));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

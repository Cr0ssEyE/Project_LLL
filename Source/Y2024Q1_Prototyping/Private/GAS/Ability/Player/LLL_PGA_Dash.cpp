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
	bIsInputPressed = false;
}

void ULLL_PGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(TEXT("DashMontage")), DashAnimMontage);
	MontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_Dash::OnCompleteCallBack);
	MontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_Dash::OnInterruptedCallBack);
	MontageTask->ReadyForActivation();

	ALLL_PlayerBase * PlayerCharacter = Cast<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));
	if(IsValid(PlayerCharacter) && IsValid(PlayerAttributeSet))
	{
		DashSpeed = PlayerAttributeSet->GetDashSpeed();
		MaxDashCount = PlayerAttributeSet->GetMaxDashCount();
	}
	bIsInputPressed = true;
	DashActionEvent();
}

void ULLL_PGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerBase * PlayerCharacter = Cast<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	}
	CurrentDashCount = 0;
	bIsInputPressed = false;
	DashInputTimerHandle.Invalidate();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_Dash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	if(DashInputTimerHandle.IsValid())
	{
		bIsInputPressed = false;
	}
	else
	{
		bIsInputPressed = true;
	}
}

void ULLL_PGA_Dash::DashActionEvent()
{
	DashInputTimerHandle.Invalidate();
	ALLL_PlayerBase * PlayerCharacter = Cast<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));
	
	if(IsValid(PlayerCharacter) && IsValid(PlayerAttributeSet) && bIsInputPressed && CurrentDashCount < MaxDashCount)
	{
		CurrentDashCount++;
		DashSpeed = PlayerAttributeSet->GetDashSpeed();
		MaxDashCount = PlayerAttributeSet->GetMaxDashCount();
		PlayerCharacter->LaunchCharacter(PlayerCharacter->GetActorForwardVector() * (DashSpeed * 1000.f), true, true);
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
		StartDashInputCheck();
		MontageJumpToSection(SECTION_DASH);
		
		bIsInputPressed = false;
	}
}

void ULLL_PGA_Dash::StartDashInputCheck()
{
	if(CurrentDashCount < MaxDashCount)
	{
		GetWorld()->GetTimerManager().SetTimer(DashInputTimerHandle, this , &ULLL_PGA_Dash::DashActionEvent, DashInputCheckTime, false, DashInputCheckDelay);
	}
}

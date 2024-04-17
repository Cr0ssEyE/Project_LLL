// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_Dash.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

ULLL_PGA_Dash::ULLL_PGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentDashCount = 0;
}

void ULLL_PGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerDashDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 어빌리티 발동")));
		}
	}
#endif
	
	// ASC에 등록된 어트리뷰트 가져오고 GA에서 필요한 어트리뷰트 저장하기
	const ALLL_PlayerBase * PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
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
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerDashDebug())
		{
			if(bWasCancelled)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("대쉬 어빌리티 중단됨")));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 어빌리티 종료")));
			}
		}
	}
#endif

	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	}
	CurrentDashCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(WaitInputTimerHandle);
	WaitInputTimerHandle.Invalidate();

	ULLL_PlayerAnimInstance* PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(PlayerCharacter->GetCharacterAnimInstance());
	PlayerAnimInstance->SetDash(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_Dash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	// FTimerHandle.IsValid = 타이머가 실행중인 경우라고 봐도 됩니다.
	// 해당 코드에서 타이머가 실행중인 경우 = 첫 대쉬 입력이 들어오고 입력이 더 안들어오나 확인하는 중
	const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(WaitInputTimerHandle);
	if(CurrentDashCount < MaxDashCount && ElapsedTime > DashReActionTime)
	{
		bIsInputPressed = true;
		DashActionEvent();
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 추가 입력 인식")));
			}
		}
#endif
	}
}

void ULLL_PGA_Dash::DashActionEvent()
{
	// TODO: MovementComponent의 LaunchCharacter 기반 물리에서 위치 기반으로 변경
	GetWorld()->GetTimerManager().ClearTimer(WaitInputTimerHandle);
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if (IsValid(PlayerCharacter) && bIsInputPressed && CurrentDashCount < MaxDashCount)
	{
		CurrentDashCount++;
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
		PlayerCharacter->GetMovementComponent()->Velocity = FVector::Zero();
		
		FVector LaunchDirection;
		if (PlayerCharacter->GetMoveInputPressed())
		{
			LaunchDirection = PlayerCharacter->GetMoveInputDirection().GetSafeNormal2D();
		}
		else
		{
			LaunchDirection = PlayerCharacter->GetActorForwardVector().GetSafeNormal2D();
		}
		
		PlayerCharacter->LaunchCharacter(LaunchDirection * (DashSpeed * 1000.f), true, true);
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
		// 애님 몽타주 처음부터 다시 실행하거나 특정 시간부터 실행 시키도록 하는게 상당히 귀찮아서 땜빵 처리
		PlayerCharacter->StopAnimMontage(DashAnimMontage);
		PlayerCharacter->PlayAnimMontage(DashAnimMontage);
		
		// 여기서 타이머 델리게이트로 호출해서 다음 입력까지 대기시간을 주면 대쉬를 연타했을 때 낭비를 줄이도록 할 수 있습니다.
		StartDashInputWait();
		bIsInputPressed = false;

		ULLL_PlayerAnimInstance* PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(PlayerCharacter->GetCharacterAnimInstance());
		PlayerAnimInstance->SetDash(true);
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 액션 발동. 현재 횟수: %d, 최대 횟수: %d"), CurrentDashCount, MaxDashCount));
			}
		}
#endif
	}
}

void ULLL_PGA_Dash::StartDashInputWait()
{
	// 대쉬 입력 기다리는 시간. 만약 시간이 초과될 경우 어빌리티 종료하고 쿨타임 적용.
	// 쿨타임의 경우에는 BP에서 지정 가능합니다.
	GetWorld()->GetTimerManager().SetTimer(WaitInputTimerHandle, this, &ULLL_PGA_Dash::EndDashInputWait, DashInputCheckTime);
}

void ULLL_PGA_Dash::EndDashInputWait()
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerDashDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 어빌리티 종료")));
		}
	}
#endif
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

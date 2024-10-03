// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_Dash.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "Util/LLL_MathHelper.h"

ULLL_PGA_Dash::ULLL_PGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentDashCount = 0;
}

void ULLL_PGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckPlayerDashDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 어빌리티 발동")));
		}
	}
#endif
	
	// ASC에 등록된 어트리뷰트 가져오고 GA에서 필요한 어트리뷰트 저장하기
	const ALLL_PlayerBase * Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(Player->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	if(IsValid(Player) && IsValid(PlayerAttributeSet))
	{
		MaxDashCount = PlayerAttributeSet->GetMaxDashCount();
		
		// 다중 번개 이누리아
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_DOUBLE_DASH))
		{
			MaxDashCount++;
		}
	}
	bIsInputPressed = true;
	MontageStop();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_Dash::DashActionEvent);
}

void ULLL_PGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckPlayerDashDebug())
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
	CurrentDashCount = 0;

	if (IsValid(DashTask) && DashTask->IsActive())
	{
		DashTask->EndTask();
	}

	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	
	ULLL_PlayerAnimInstance* PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(PlayerCharacter->GetCharacterAnimInstance());
	PlayerAnimInstance->SetDash(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_Dash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	if (IsValid(WaitTagTask) && WaitTagTask->IsActive())
	{
		bIsInputPressed = true;
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(DebugGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 추가 입력 인식")));
			}
		}
#endif
	}
}

void ULLL_PGA_Dash::DashActionEvent()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	if (IsValid(Player) && bIsInputPressed && CurrentDashCount < MaxDashCount)
	{
		bIsInputPressed = false;
		CurrentDashCount++;
		
		FVector DashDirection;
		if (Player->GetMoveInputPressed())
		{
			DashDirection = Player->GetMoveInputDirection().GetSafeNormal2D();
		}
		else
		{
			DashDirection = Player->GetActorForwardVector().GetSafeNormal2D();
		}
		
		const float OldDashDistance = PlayerAttributeSet->GetDashDistance();
		
		float DashDistance = OldDashDistance;

		// 다중 번개 이누리아
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_DOUBLE_DASH) && Player->GetHorseEnuriaCount() < Player->GetDoubleDashHorseEnuriaCheckCount())
		{
			DashDistance *= Player->GetDoubleDashDashRate();
		}
		DashDistance += PlayerAttributeSet->GetDashDistancePlus();

		const float NewDashDistance = DashDistance;
		const float DashCorrectionDistance = PlayerAttributeSet->GetDashCorrectionDistance() + NewDashDistance - OldDashDistance;
		const float DashSpeed = PlayerAttributeSet->GetDashSpeed() * (NewDashDistance / OldDashDistance);

		const FVector DashLocation = FLLL_MathHelper::CalculatePlayerLaunchableLocation(GetWorld(), Player, DashDistance, DashCorrectionDistance, DashDirection);
		Player->GetMovementComponent()->Velocity = FVector::Zero();
		Player->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER_EVADE);
		
		if (IsValid(DashTask) && DashTask->IsActive())
		{
			DashTask->EndTask();
		}
		UE_LOG(LogTemp, Log, TEXT("대시 거리 : %f"), DashDistance)
		DashTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("Dash"), DashLocation, DashDistance / DashSpeed, nullptr, nullptr);
		DashTask->OnTargetLocationReached.AddDynamic(this, &ULLL_PGA_Dash::LocationReachedEvent);
		DashTask->ReadyForActivation();

		if (IsValid(WaitTagTask) && WaitTagTask->IsActive())
		{
			WaitTagTask->EndTask();
		}
		WaitTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_PLAYER_STATE_INPUT_CHECK_DASH, nullptr, true);
		WaitTagTask->EventReceived.AddDynamic(this, &ULLL_PGA_Dash::CheckInputPressed);
		WaitTagTask->ReadyForActivation();
		
		// 애님 몽타주 처음부터 다시 실행하거나 특정 시간부터 실행 시키도록 하는게 상당히 귀찮아서 땜빵 처리
		Player->StopAnimMontage(DashAnimMontage);
		Player->PlayAnimMontage(DashAnimMontage);

		ULLL_PlayerAnimInstance* PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(Player->GetCharacterAnimInstance());
		PlayerAnimInstance->SetDash(true);

		const FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), TAG_GAS_PLAYER_DASH_START, PayloadData);
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(DebugGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 액션 발동. 현재 횟수: %d, 최대 횟수: %d, 대시 거리: %f"), CurrentDashCount, MaxDashCount, DashDistance));
			}
		}
#endif
	}
}

void ULLL_PGA_Dash::LocationReachedEvent()
{
	UE_LOG(LogTemp, Log, TEXT("대시 종료"))
	const FGameplayEventData PayloadData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), TAG_GAS_PLAYER_DASH_END, PayloadData);

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
	UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();

	// 재빠른 몸놀림 이누리아
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_EVASION_DASH))
	{
		FGameplayEffectContextHandle EffectContextHandle = PlayerASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = PlayerASC->MakeOutgoingSpec(PlayerDataAsset->EvasionDashEvasionEffect, GetAbilityLevel(), EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			const ULLL_ExtendedGameplayEffect* EvasionDashEvasionEffect = CastChecked<ULLL_ExtendedGameplayEffect>(PlayerDataAsset->EvasionDashEvasionEffect.GetDefaultObject());
			const FAbilityDataTable* AbilityData = EvasionDashEvasionEffect->GetAbilityData();
			const float MagnitudeValue1 = AbilityData->AbilityValue1 * GetAbilityLevel() / static_cast<uint32>(AbilityData->Value1Type);
			float MagnitudeValue2 = AbilityData->AbilityValue2 * GetAbilityLevel() / static_cast<uint32>(AbilityData->Value2Type);
			if (Player->GetHorseEnuriaCount() >= Player->GetEvasionDashHorseEnuriaCheckCount())
			{
				MagnitudeValue2 = 1.0f;
			}
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_1, MagnitudeValue1);
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_2, MagnitudeValue2);
			PlayerASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void ULLL_PGA_Dash::CheckInputPressed(FGameplayEventData EventData)
{
 	if (!bIsInputPressed || CurrentDashCount >= MaxDashCount)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_Dash::DashActionEvent);
}

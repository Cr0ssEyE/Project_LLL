// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

#include "AbilitySystemComponent.h"
#include "BrainComponent.h"
#include "GameplayAbilitySpec.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Ability/Monster/LLL_MGA_GroundStrike.h"
#include "GAS/Ability/Monster/LLL_MonsterGameplayAbilityBase.h"

ALLL_MonsterBase::ALLL_MonsterBase()
{
	GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ALLL_MonsterBase::BeginPlay()
{
	Super::BeginPlay();

	MonsterBaseDataAsset = Cast<ULLL_MonsterBaseDataAsset>(CharacterDataAsset);

	if(IsValid(ASC))
	{
		if(IsValid(MonsterBaseDataAsset->DefaultGameplayAbility_Test))
		{
			FGameplayAbilitySpec AbilitySpec(MonsterBaseDataAsset->DefaultGameplayAbility_Test);
			ASC->GiveAbility(AbilitySpec);
		}
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		ProtoGameInstance->MonsterToggleAIDelegate.AddDynamic(this, &ALLL_MonsterBase::ToggleAIHandle);
		ProtoGameInstance->MonsterToggleAIDelegate.Broadcast(ProtoGameInstance->GetMonsterToggleAIDebug());
	}
#endif
}

void ALLL_MonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckMonsterCollisionDebug())
		{
			GetCapsuleComponent()->SetHiddenInGame(false);
		}
		else
		{
			GetCapsuleComponent()->SetHiddenInGame(true);
		}
	}
#endif
}

void ALLL_MonsterBase::Dead()
{
	Super::Dead();

	const ALLL_MonsterBaseAIController* MonsterBaseAIController = Cast<ALLL_MonsterBaseAIController>(GetController());
	if (IsValid(MonsterBaseAIController))
	{
		MonsterBaseAIController->GetBrainComponent()->StopLogic("Monster Is Dead");
	}
}

void ALLL_MonsterBase::Attack()
{
	Super::Attack();

	/*ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(MonsterBaseAnimInstance))
	{
		MonsterBaseAnimInstance->PlayAttackAnimation();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 공격 수행"), *GetName()));
			}
		}
#endif
	}*/

	FGameplayAbilitySpec* SkillSpec = ASC->FindAbilitySpecFromClass(MonsterBaseDataAsset->DefaultGameplayAbility_Test);
	if (SkillSpec)
	{
		if (!SkillSpec->IsActive())
		{
			ASC->TryActivateAbility(SkillSpec->Handle);
		}
	}
}

void ALLL_MonsterBase::Damaged()
{
	ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(MonsterBaseAnimInstance))
	{
		MonsterBaseAnimInstance->PlayDamagedAnimation();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterCollisionDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 피격"), *GetName()));
			}
		}
#endif
	}
}

bool ALLL_MonsterBase::CanPlayAttackAnimation()
{
	if (IsValid(CharacterAnimInstance))
	{
		if (CharacterAnimInstance->Montage_IsPlaying(CharacterDataAsset->AttackAnimMontage))
		{
			return false;
		}

		if (CharacterAnimInstance->Montage_IsPlaying(MonsterBaseDataAsset->DamagedAnimMontage))
		{
			return false;
		}

		return true;
	}

	return false;
}

void ALLL_MonsterBase::ToggleAIHandle(bool value)
{
	const ALLL_MonsterBaseAIController* MonsterBaseAIController = Cast<ALLL_MonsterBaseAIController>(GetController());
	if (IsValid(MonsterBaseAIController))
	{
		UBrainComponent* BrainComponent = MonsterBaseAIController->GetBrainComponent();
		if (value)
		{
			BrainComponent->ResumeLogic(TEXT("AI Debug Is Activated"));
		}
		else
		{
			BrainComponent->PauseLogic(TEXT("AI Debug Is Deactivated"));
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

ALLL_MonsterBase::ALLL_MonsterBase()
{
	GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ALLL_MonsterBase::BeginPlay()
{
	Super::BeginPlay();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		ProtoGameInstance->MonsterToggleAIDelegate.AddDynamic(this, &ALLL_MonsterBase::ToggleAIHandle);
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

float ALLL_MonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(CurrentShieldAmount > 0)
	{
		CurrentShieldAmount -= DamageAmount;
		if(CurrentShieldAmount <= 0)
		{
			CurrentShieldAmount = 0;
		}
	}
	else
	{
		CurrentHealthAmount -= DamageAmount;
		if(CurrentHealthAmount <= 0)
		{
			CurrentHealthAmount = 0;
			Dead();
		}
		else
		{
			Damaged();
		}
	}
	
	return 0;
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

	ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(MonsterBaseAnimInstance))
	{
		MonsterBaseAnimInstance->PlayAttackAnimation();
	}
}

bool ALLL_MonsterBase::AttackAnimationIsPlaying()
{
	if (IsValid(CharacterAnimInstance))
	{
		return CharacterAnimInstance->Montage_IsPlaying(CharacterDataAsset->AttackAnimMontage);
	}

	return false;
}

void ALLL_MonsterBase::Damaged()
{
	// Todo: 피격 애니메이션 재생
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

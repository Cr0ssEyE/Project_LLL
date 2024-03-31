// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

#include "AbilitySystemComponent.h"
#include "BrainComponent.h"
#include "GameplayAbilitySpec.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/DropGold/LLL_DropGoldAttributeSet.h"
#include "GAS/Attribute/Monster/LLL_MonsterAttributeSet.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MonsterBase::ALLL_MonsterBase()
{
	GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DropGoldAttributeSet = CreateDefaultSubobject<ULLL_DropGoldAttributeSet>(TEXT("DropGoldAttribute"));
	DropGoldEffect = FLLLConstructorHelper::FindAndGetClass<UGameplayEffect>(TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/DropGold/BPGE_DropGold.BPGE_DropGold_C'"), EAssertionLevel::Check);
	
}

void ALLL_MonsterBase::BeginPlay()
{
	Super::BeginPlay();
	ASC->AddSpawnedAttribute(DropGoldAttributeSet);
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(DropGoldEffect, 1.0, EffectContextHandle);
	if(EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
	
	MonsterBaseDataAsset = Cast<ULLL_MonsterBaseDataAsset>(CharacterDataAsset);

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
	//DropGold() -> GoldAttribute -> DropGoldStat -> player Gold up
	DropGold(FGameplayTag::RequestGameplayTag(FName("System.Drop.Gold")), 0);
	const ALLL_MonsterBaseAIController* MonsterBaseAIController = Cast<ALLL_MonsterBaseAIController>(GetController());
	if (IsValid(MonsterBaseAIController))
	{
		MonsterBaseAIController->GetBrainComponent()->StopLogic("Monster Is Dead");
	}
	
}

void ALLL_MonsterBase::Attack()
{
	Super::Attack();

	int32 index = FMath::RandRange(0, MonsterBaseDataAsset->ActiveGameplayAbility.Num() - 1);
	FGameplayAbilitySpec* SkillSpec = ASC->FindAbilitySpecFromClass(MonsterBaseDataAsset->ActiveGameplayAbility[index]);
	if (SkillSpec)
	{
		if (!SkillSpec->IsActive())
		{
			if (ASC->TryActivateAbility(SkillSpec->Handle))
			{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
				if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
				{
					if (ProtoGameInstance->CheckMonsterAttackDebug())
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 공격 수행"), *GetName()));
					}
				}
#endif
			}
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

void ALLL_MonsterBase::DropGold(const FGameplayTag tag, int32 data)
{
	float GoldData = DropGoldAttributeSet->GetDropGoldStat();

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	for (UActorComponent* ChildComponent : Player->GetComponents())
	{
		ULLL_PlayerGoldComponet* GoldComponet = Cast<ULLL_PlayerGoldComponet>(ChildComponent);
		if(IsValid(GoldComponet))
		{
			GoldComponet->IncreaseMoney(GoldData);
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("PlayerGold %f"), GoldComponet->GetMoney()));
				}
			}
#endif
		}
	}
}

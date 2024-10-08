// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"

#include "AbilitySystemComponent.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Entity/Character/Player/LLL_PlayerGoldComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Kismet/GameplayStatics.h"

ALLL_BreakableObjectBase::ALLL_BreakableObjectBase()
{
	BaseMesh->SetStaticMesh(FLLL_ConstructorHelper::FindAndGetObject<UStaticMesh>(PATH_BREAKABLE_OBJECT_TEST_MESH, EAssertionLevel::Check));
	SetRootComponent(BaseMesh);
	
	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollisionComponent"));
	HitCollision->SetCollisionProfileName(CP_MONSTER);
	HitCollision->SetCapsuleSize(100.0f, 100.0f, true);
	HitCollision->SetupAttachment(RootComponent);
	
	DropGoldAttributeSet = CreateDefaultSubobject<ULLL_DropGoldAttributeSet>(TEXT("DropGoldAttribute"));
	InitEffect = FLLL_ConstructorHelper::FindAndGetClass<UGameplayEffect>(PATH_BREAKABLE_OBJECT_TEST_EFFECT, EAssertionLevel::Check);
	ASC->RegisterGameplayTagEvent(TAG_GAS_SYSTEM_DROP_GOLD).AddUObject(this, &ALLL_BreakableObjectBase::DropGold);
}

void ALLL_BreakableObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);

		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		EffectContextHandle.AddInstigator(this, this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitEffect, 1.0, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void ALLL_BreakableObjectBase::DropGold(const FGameplayTag tag, int32 data)
{
	const float GoldData = DropGoldAttributeSet->GetDropGoldStat();

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	for (UActorComponent* ChildComponent : Player->GetComponents())
	{
		ULLL_PlayerGoldComponent* GoldComponent = Cast<ULLL_PlayerGoldComponent>(ChildComponent);
		if(IsValid(GoldComponent))
		{
			GoldComponent->IncreaseMoney(GoldData);
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (DebugGameInstance->CheckPlayerAttackDebug() || DebugGameInstance->CheckPlayerSkillDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("PlayerGold %f"), GoldComponent->GetMoney()));
				}
			}
#endif
		}
	}
	
	Destroy();
}

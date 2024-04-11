// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"

#include "AbilitySystemComponent.h"
#include "Util/LLLConstructorHelper.h"
#include "Entity/Character/Player/LLL_PlayerGoldComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "Kismet/GameplayStatics.h"

class UProtoGameInstance;
// Sets default values
ALLL_BreakableObjectBase::ALLL_BreakableObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BaseMesh->SetStaticMesh(FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(PATH_BREAKABLE_OBJECT_TEST_MESH, EAssertionLevel::Check));
	SetRootComponent(BaseMesh);
	
	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollisionComponent"));
	HitCollision->SetCollisionProfileName(CP_MONSTER);
	HitCollision->SetCapsuleSize(100.0f, 100.0f, true);
	HitCollision->SetupAttachment(RootComponent);
	
	DropGoldAttributeSet = CreateDefaultSubobject<ULLL_DropGoldAttributeSet>(TEXT("DropGoldAttribute"));
	InitEffect = FLLLConstructorHelper::FindAndGetClass<UGameplayEffect>(PATH_BREAKABLE_OBJECT_TEST_EFFECT, EAssertionLevel::Check);
	ASC->RegisterGameplayTagEvent(TAG_GAS_SYSTEM_DROP_GOLD).AddUObject(this, &ALLL_BreakableObjectBase::DropGold);
}

// Called when the game starts or when spawned
void ALLL_BreakableObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);

		// GE 기반으로 자신의 어트리뷰트 초기화
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitEffect, 1.0, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

// Called every frame
void ALLL_BreakableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALLL_BreakableObjectBase::DropGold(const FGameplayTag tag, int32 data)
{
	float GoldData = DropGoldAttributeSet->GetDropGoldStat();

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	for (UActorComponent* ChildComponent : Player->GetComponents())
	{
		ULLL_PlayerGoldComponent* GoldComponet = Cast<ULLL_PlayerGoldComponent>(ChildComponent);
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
	Destroy();
}

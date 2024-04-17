// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_InteractiveObject.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"

ALLL_InteractiveObject::ALLL_InteractiveObject()
{
	PrimaryActorTick.bCanEverTick = true;

	//BaseObjectDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_InteractiveObjectDataAsset>(PATH_INTERACTIVE_OBJECT_DATA, EAssertionLevel::Check);

	InteractOnlyCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Ineractive Collision"));
	InteractOnlyCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractOnlyCollisionBox->SetCollisionProfileName(CP_INTERACTION);
	InteractOnlyCollisionBox->SetupAttachment(RootComponent);
}

void ALLL_InteractiveObject::BeginPlay()
{
	Super::BeginPlay();

}

void ALLL_InteractiveObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(OtherActor))
	{
		PlayerCharacter->AddInteractiveObject(this);
	}
}

void ALLL_InteractiveObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if(ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(OtherActor))
	{
		PlayerCharacter->RemoveInteractiveObject(this);
	}
}

void ALLL_InteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectCollisionDebug())
		{
			InteractOnlyCollisionBox->SetHiddenInGame(false);
		}
		else
		{
			InteractOnlyCollisionBox->SetHiddenInGame(true);
		}
	}
#endif
}

void ALLL_InteractiveObject::InteractiveEvent()
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("오브젝트 상호작용 실행")));
		}
	}
#endif
}



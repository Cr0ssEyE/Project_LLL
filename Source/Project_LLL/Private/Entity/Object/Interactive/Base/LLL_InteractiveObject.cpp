// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_InteractiveObjectDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_InteractiveObject::ALLL_InteractiveObject()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractiveObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_InteractiveObjectDataAsset>(PATH_INTERACTIVE_OBJECT_DATA, EAssertionLevel::Check);

	InteractOnlyCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Ineractive Collision"));
	InteractOnlyCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractOnlyCollisionBox->SetBoxExtent(InteractiveObjectDataAsset->InteractOnlyCollisionBoxExtent);
	InteractOnlyCollisionBox->SetCollisionProfileName(CP_INTERACTION);
	InteractOnlyCollisionBox->SetupAttachment(RootComponent);
}

void ALLL_InteractiveObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(OtherActor))
	{
		PlayerCharacter->AddInteractiveObject(this);
	}
}

void ALLL_InteractiveObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(OtherActor))
	{
		PlayerCharacter->RemoveInteractiveObject(this);
	}
}

void ALLL_InteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckObjectCollisionDebug())
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
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("오브젝트 상호작용 실행")));
		}
	}
#endif
}



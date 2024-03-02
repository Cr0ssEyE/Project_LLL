// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "DataAsset/LLL_PlayerWireObjectDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"
#include "Util/LLLConstructorHelper.h"


// Sets default values
ALLL_PlayerWireHand::ALLL_PlayerWireHand()
{
	BaseObjectDataAsset = Cast<ULLL_BaseObjectDataAsset>(FLLLConstructorHelper::FindAndGetObject<ULLL_PlayerWireObjectDataAsset>(PATH_PLAYER_WIRE_DATA, EAssertionLevel::Check));
	WireHandAttributeSet = CreateDefaultSubobject<ULLL_PlayerWireHandAttributeSet>(TEXT("WireHandAttributeSet"));
}

void ALLL_PlayerWireHand::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->AddSpawnedAttribute(WireHandAttributeSet);
}

// Called when the game starts or when spawned
void ALLL_PlayerWireHand::BeginPlay()
{
	Super::BeginPlay();
	if(ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetParentActor()))
	{
		SetOwner(PlayerCharacter);
	}
}

void ALLL_PlayerWireHand::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OtherActor);
	if(IsValid(Monster))
	{
		// PGA_WireHandGrab
		FGameplayTagContainer GrabTag;
		ASC->TryActivateAbilitiesByTag(GrabTag);
	}
}

void ALLL_PlayerWireHand::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}


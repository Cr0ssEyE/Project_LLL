// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Skill/Base/LLL_SkillObject.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "DataAsset/LLL_SkillObjectDataAsset.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/Character.h"
#include "GAS/Attribute/Object/Skill//Base/LLL_SkillObjectAttributeSet.h"

ALLL_SkillObject::ALLL_SkillObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SkillCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Collision"));
	SkillCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SkillCollisionBox->SetCollisionProfileName(CP_PLAYER_SKILL);
	SkillCollisionBox->SetupAttachment(RootComponent);
}

void ALLL_SkillObject::BeginPlay()
{
	Super::BeginPlay();

	SkillObjectDataAsset = Cast<ULLL_SkillObjectDataAsset>(BaseObjectDataAsset);
	ASC->AddSpawnedAttribute(SkillObjectAttributeSet);

	SetOwner(GetWorld()->GetFirstPlayerController()->GetCharacter());
	SkillCollisionBox->SetBoxExtent(SkillObjectDataAsset->OverlapCollisionSize);

	UE_LOG(LogTemp, Log, TEXT("%s 생성"), *GetName())
	
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		UE_LOG(LogTemp, Log, TEXT("%s 파괴"), *GetName())
		Destroy();
	}), SkillObjectAttributeSet->GetDestroyTimer(), false);
}

void ALLL_SkillObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UE_LOG(LogTemp, Log, TEXT("%s에게 스킬 상호작용 발생"), *OtherActor->GetName())
}

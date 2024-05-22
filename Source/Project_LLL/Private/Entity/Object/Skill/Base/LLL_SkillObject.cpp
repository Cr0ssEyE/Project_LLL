// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Skill/Base/LLL_SkillObject.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "DataAsset/LLL_SkillObjectDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
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
	
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		Destroy();
	}), SkillObjectAttributeSet->GetDestroyTimer(), false);
}

void ALLL_SkillObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(SkillObjectDataAsset->DamageEffect, 1.0, EffectContextHandle);
	if(EffectSpecHandle.IsValid())
	{
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
		{
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
		}
	}
}

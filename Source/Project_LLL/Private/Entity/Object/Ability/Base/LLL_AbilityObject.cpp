// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"

#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/LLL_AbilityObjectDataAsset.h"
#include "GameFramework/Character.h"
#include "GAS/Attribute/Object/Ability/Base/LLL_AbilityObjectAttributeSet.h"
#include "Util/LLL_MathHelper.h"

ALLL_AbilityObject::ALLL_AbilityObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	OverlapCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Collision"));
	OverlapCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapCollisionSphere->SetCollisionProfileName(CP_PLAYER_ABILITY_OBJECT);
	OverlapCollisionSphere->SetupAttachment(RootComponent);
}

void ALLL_AbilityObject::BeginPlay()
{
	Super::BeginPlay();

	AbilityObjectDataAsset = Cast<ULLL_AbilityObjectDataAsset>(BaseObjectDataAsset);

	SetOwner(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	OverlapCollisionSphere->SetSphereRadius(AbilityObjectDataAsset->OverlapCollisionRadius);

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]{
		FTimerHandle DestroyTimerHandle;
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
			Destroy();
		}), AbilityObjectAttributeSet->GetDestroyTimer(), false);
	}));
}

void ALLL_AbilityObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	UE_LOG(LogTemp, Log, TEXT("%s와 겹침"), *OtherActor->GetName())
}

void ALLL_AbilityObject::DamageToOverlapActor(AActor* OtherActor)
{
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, OtherActor]{
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
		{
			float OffencePower = AbilityObjectAttributeSet->GetOffensePower();
			
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(AbilityObjectDataAsset->DamageEffect, AbilityLevel, EffectContextHandle);
			if(EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_1, OffencePower);
				UE_LOG(LogTemp, Log, TEXT("%s에게 데미지"), *OtherActor->GetName())
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
			}
		}
	}));
}

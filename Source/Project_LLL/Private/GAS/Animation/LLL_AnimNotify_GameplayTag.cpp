// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Animation/LLL_AnimNotify_GameplayTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

ULLL_AnimNotify_GameplayTag::ULLL_AnimNotify_GameplayTag() :
	NotifyName(GetClass()->GetName()),
	bIsUsingTagTrigger(true),
	NotifyLevel(1.f),
	bIsUsingGiveTag(false),
	GiveTagCount(1.f),
	bIsUsingRemoveTag(false),
	RemoveTagCount(1.f)
{
	
}

void ULLL_AnimNotify_GameplayTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();

		if (bIsUsingTagTrigger && TriggerGameplayTag.IsValid())
		{
			Notify_TagEventTriggered(OwnerActor);
		}

		if ((bIsUsingGiveTag && OwnerGiveTag.IsValid()) || (bIsUsingRemoveTag && OwnerRemoveTag.IsValid()))
		{
			Notify_TagAddOrRemove(OwnerActor);
		}
	}
}

FString ULLL_AnimNotify_GameplayTag::GetNotifyName_Implementation() const
{
	FString Name = Super::GetNotifyName_Implementation();

	if (NotifyName != TEXT(""))
	{
		return NotifyName;
	}
	return Name;
}

void ULLL_AnimNotify_GameplayTag::Notify_TagEventTriggered(AActor* OwnerActor)
{
	if (OwnerActor)
	{
		FGameplayEventData PayloadData;
		PayloadData.EventMagnitude = NotifyLevel;
		for (const auto GameplayTag : TriggerGameplayTag.GetGameplayTagArray())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, GameplayTag, PayloadData);
		}
	}
}

void ULLL_AnimNotify_GameplayTag::Notify_TagAddOrRemove(AActor* OwnerActor)
{
	const IAbilitySystemInterface* ASCOwner = Cast<IAbilitySystemInterface>(OwnerActor);
	if (ASCOwner && OwnerGiveTag.IsValid())
	{
		ASCOwner->GetAbilitySystemComponent()->AddLooseGameplayTags(OwnerGiveTag, GiveTagCount);
	}

	if (ASCOwner && OwnerRemoveTag.IsValid())
	{
		ASCOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTags(OwnerRemoveTag, RemoveTagCount);
	}
}

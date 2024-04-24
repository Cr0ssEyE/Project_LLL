// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Animation/LLL_AnimNotify_GameplayTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

ULLL_AnimNotify_GameplayTag::ULLL_AnimNotify_GameplayTag()
{
	
}

void ULLL_AnimNotify_GameplayTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor && TriggerGameplayTag.IsValid())
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = NotifyLevel;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}

		const IAbilitySystemInterface* ASCOwner = Cast<IAbilitySystemInterface>(OwnerActor);
		if (ASCOwner && OwnerGiveTag.IsValid())
		{
			ASCOwner->GetAbilitySystemComponent()->AddLooseGameplayTag(OwnerGiveTag);
		}
	}
}

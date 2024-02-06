// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LLL_AnimNotify_GameplayTag.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_AnimNotify_GameplayTag : public UAnimNotify
{
	GENERATED_BODY()

public:
	ULLL_AnimNotify_GameplayTag();
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, Meta=(Categories=Event))
	FGameplayTag TriggerGameplayTag;
	
	UPROPERTY(EditAnywhere)
	float NotifyLevel = 0.0f;
	
};

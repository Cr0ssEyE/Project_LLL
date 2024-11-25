// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODAnimNotifyPlay.h"
#include "LLL_AnimNotify_FMod.generated.h"

enum class EFModParameter : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AnimNotify_FMod : public UFMODAnimNotifyPlay
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = "FMOD Anim Notify")
	uint8 bStop : 1;
	
	UPROPERTY(EditAnywhere, Category = "FMOD Anim Notify", meta=(EditCondition = "bStop == false", EditConditionHides))
	EFModParameter Parameter;
};

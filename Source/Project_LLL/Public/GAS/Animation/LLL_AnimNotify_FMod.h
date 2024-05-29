// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LLL_AnimNotify_FMod.generated.h"

enum class EFModParameter : uint8;
class UFMODEvent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AnimNotify_FMod : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditAnywhere, DisplayName = "FMOD 이벤트")
	TObjectPtr<UFMODEvent> FModEvent;
	
	UPROPERTY(EditAnywhere, DisplayName = "FMOD 파라미터")
	EFModParameter FModParameter;
};

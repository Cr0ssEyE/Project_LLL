// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_PlayNiagaraEffect.h"
#include "LLL_AnimNotify_Niagara.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AnimNotify_Niagara : public UAnimNotify_PlayNiagaraEffect
{
	GENERATED_BODY()

protected:
	virtual UFXSystemComponent* SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, DisplayName = "이펙트 제거")
	uint8 bRemoveEffect : 1;
};

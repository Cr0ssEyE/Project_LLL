// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_TimedNiagaraEffect.h"
#include "LLL_AnimNotify_TimedNiagara.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AnimNotify_TimedNiagara : public UAnimNotifyState_TimedNiagaraEffectAdvanced
{
	GENERATED_BODY()

	ULLL_AnimNotify_TimedNiagara();
	
protected:
	virtual UFXSystemComponent* SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NiagaraSystem)
	FVector Scale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NiagaraSystem)
	bool bAbsoluteScale;
};

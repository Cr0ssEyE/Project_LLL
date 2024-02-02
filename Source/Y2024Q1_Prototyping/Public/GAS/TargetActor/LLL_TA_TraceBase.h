// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "LLL_TA_TraceBase.generated.h"

UENUM()
enum class EShapeTypes
{
	Box,
	Capsule,
	Sphere
};

UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_TA_TraceBase : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_TA_TraceBase();

protected:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Shape")
	EShapeTypes TraceShape;

	UPROPERTY(EditDefaultsOnly, Category = "Shape", meta=(EditCondition = "TraceShape == EShapeTypes::Box", EditConditionHides))
	FVector3d BoxExtents;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shape", meta=(EditCondition = "TraceShape == EShapeTypes::Capsule", EditConditionHides))
	FVector2D CapsuleSize;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shape", meta=(EditCondition = "TraceShape == EShapeTypes::Sphere", EditConditionHides))
	float SphereRadius;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Enumeration/LLL_EditorSelectHelper.h"
#include "LLL_TA_TraceBase.generated.h"

UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_TA_TraceBase : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_TA_TraceBase();

	void SetTraceInfo(const ESelectTraceTarget NewTraceTarget, const ESelectShapeTypes NewShapeTypes = ESelectShapeTypes::Box, FVector Extents = FVector::ZeroVector);
	void SetTraceInfo(const ESelectTraceTarget NewTraceTarget, const ESelectShapeTypes NewShapeTypes = ESelectShapeTypes::Capsule, FVector2D Radius = FVector2D::ZeroVector);
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
	virtual FGameplayAbilityTargetDataHandle TraceResult() const;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	ESelectTraceTarget TraceTarget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	FVector TraceStartLocation = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	FVector TraceEndLocation = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shape")
	ESelectShapeTypes BaseShape;

	UPROPERTY(EditDefaultsOnly, Category = "Shape", meta=(EditCondition = "BaseShape == ESelectShapeTypes::Box", EditConditionHides))
	FVector BoxExtents;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shape", meta=(EditCondition = "BaseShape == ESelectShapeTypes::Capsule", EditConditionHides))
	FVector2D CapsuleExtents;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shape", meta=(EditCondition = "BaseShape == ESelectShapeTypes::Sphere", EditConditionHides))
	FVector2D SphereExtents;

	mutable FCollisionShape TraceShape;
	
};

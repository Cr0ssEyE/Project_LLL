// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Enumeration/LLL_EditorSelectHelper.h"
#include "LLL_TA_TraceBase.generated.h"

UCLASS()
class PROJECT_LLL_API ALLL_TA_TraceBase : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_TA_TraceBase();

	void SetTraceInfo(const ESelectTraceTarget NewTraceTarget, const ESelectShapeTypes NewShapeTypes = ESelectShapeTypes::Box, FVector Extents = FVector::ZeroVector);
	void SetTraceInfo(const ESelectTraceTarget NewTraceTarget, const ESelectShapeTypes NewShapeTypes = ESelectShapeTypes::Capsule, FVector2D Extents = FVector2D::ZeroVector);
	void SetTraceInfo(const ESelectTraceTarget NewTraceTarget, const ESelectShapeTypes NewShapeTypes = ESelectShapeTypes::Sphere, float Radius = 0.0f);
	void SetTraceInfo(const ESelectTraceTarget NewTraceTarget, const ESelectShapeTypes NewShapeTypes = ESelectShapeTypes::Cone, float Distance = 0.0f, float FieldOfView = 0.0f);
	
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
	float SphereRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Shape", meta=(EditCondition = "BaseShape == ESelectShapeTypes::Cone", EditConditionHides))
	float ConeDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Shape", meta=(EditCondition = "BaseShape == ESelectShapeTypes::Cone", EditConditionHides))
	float ConeFieldOfView;

	mutable FCollisionShape TraceShape;

	mutable ECollisionChannel TraceChannel;
};

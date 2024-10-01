// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LLL_InstancedMeshObject.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class PROJECT_LLL_API ALLL_InstancedMeshObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALLL_InstancedMeshObject();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TArray<FTransform> Instances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TArray<FVector> Colors;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> Plane;

	UFUNCTION(BlueprintCallable)
	void SetInstance();

	uint8 InstanceCount;
	uint8 ArrayCount;
};

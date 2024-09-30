// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Base/LLL_InstancedMeshObject.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Util/LLL_ConstructorHelper.h"

// Sets default values
ALLL_InstancedMeshObject::ALLL_InstancedMeshObject()
{
	Plane = FLLL_ConstructorHelper::FindAndGetObject<UStaticMesh>(PATH_2D_RESOURCE_PLANE, EAssertionLevel::Check);
	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	InstancedStaticMesh->SetStaticMesh(Plane);
	InstancedStaticMesh->SetCollisionProfileName(CP_NO_COLLISION);
	SetRootComponent(InstancedStaticMesh);
	
	FTransform Transform = FTransform(FVector(0.0f, 0.0f, 0.0f));
	InstancedStaticMesh->AddInstance(Transform);
	Instances.Add(Transform);
	Colors.Add(FVector(1.0f, 1.0f, 1.0f));
}

void ALLL_InstancedMeshObject::SetInstance()
{
	uint8 InstanceCount = InstancedStaticMesh->GetInstanceCount();
	uint8 ArrayCount = 0;
	if (!Instances.IsEmpty())
	{
		ArrayCount = Instances.Num();
	}
	
	if (InstanceCount > ArrayCount)
	{
		FTransform Transform;
		for (uint8 i = ArrayCount; i < InstanceCount; i++)
		{
			InstancedStaticMesh->GetInstanceTransform(ArrayCount, Transform);
			Instances.Add(Transform);

			Colors.Add(FVector(1.0f, 1.0f, 1.0f));
		}
	}
	else if (InstanceCount < ArrayCount)
	{
		for (uint8 i = InstanceCount; i < ArrayCount; i++)
		{
			Instances.Pop();
			Colors.Pop();
		}
	}
	
	InstanceCount = InstancedStaticMesh->GetInstanceCount();
	
	for (uint8 i = 0; i < InstanceCount; i++)
	{
		InstancedStaticMesh->SetCustomDataValue(i, 0, Colors[i].X, true);
		InstancedStaticMesh->SetCustomDataValue(i, 1, Colors[i].Y, true);
		InstancedStaticMesh->SetCustomDataValue(i, 2, Colors[i].Z, true);
	}
}

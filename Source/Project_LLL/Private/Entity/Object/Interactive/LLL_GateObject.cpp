// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_GateObject.h"

#include "Constant/LLL_FilePath.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_GateObject::ALLL_GateObject()
{
	GateMesh = FLLL_ConstructorHelper::FindAndGetObject<UStaticMesh>(PATH_GATE_OBJECT_TEST_MESH, EAssertionLevel::Check);
	BaseMesh->SetStaticMesh(GateMesh);
	IsGateEnabled = false;
}

void ALLL_GateObject::BeginPlay()
{
	Super::BeginPlay();
}

void ALLL_GateObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALLL_GateObject::InteractiveEvent()
{
	if(!IsGateEnabled)
	{
		return;
	}
	Super::InteractiveEvent();
	
	OpenGate();
	
	StageDestroyDelegate.Broadcast();
}

void ALLL_GateObject::OpenGate()
{
	//문 오픈 애니 및 이펙트
	AddActorLocalRotation(FRotator(0.0f, -90.0f, 0.0f));
}

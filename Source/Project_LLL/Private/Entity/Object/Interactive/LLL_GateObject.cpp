// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_GateObject.h"

#include "Constant/LLL_FilePath.h"
#include "Util/LLLConstructorHelper.h"

ALLL_GateObject::ALLL_GateObject()
{
	GateMesh = FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(PATH_GATE_OBJECT_TEST_MESH, EAssertionLevel::Check);
	BaseMesh->SetStaticMesh(GateMesh);
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
	Super::InteractiveEvent();

	OpenGate();
}

void ALLL_GateObject::NotifyActorBeginOverlap(AActor* OtherActor)
{

}

void ALLL_GateObject::OpenGate()
{
	//�� ���� �ִ� �� ����Ʈ
	AddActorLocalRotation(FRotator(0.0f, -90.0f, 0.0f));
}


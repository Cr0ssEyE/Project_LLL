// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_GateObject.h"
#include "Util/LLLConstructorHelper.h"

ALLL_GateObject::ALLL_GateObject()
{
	
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

	// 애니메이션 재생
}

void ALLL_GateObject::PostInitializeComponents()
{
	BaseMesh->SetStaticMesh(FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/MapTest/Meshes/SM_GateTest.SM_GateTest'"), EAssertionLevel::Check));
}

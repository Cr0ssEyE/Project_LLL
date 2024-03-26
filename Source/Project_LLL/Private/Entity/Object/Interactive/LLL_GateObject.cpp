// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_GateObject.h"
#include "Util/LLLConstructorHelper.h"

ALLL_GateObject::ALLL_GateObject()
{
	BaseMesh->SetStaticMesh(FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/MapTest/Meshes/SM_GateTest.SM_GateTest'"), EAssertionLevel::Check));
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

	//문 오픈 애니 및 이펙트 로직
}


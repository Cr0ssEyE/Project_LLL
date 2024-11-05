// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"

#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"

ALLL_BreakableObjectBase::ALLL_BreakableObjectBase()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->SetCollisionProfileName(CP_MONSTER);
	SetRootComponent(CapsuleComponent);

	Crack = 0;
}

void ALLL_BreakableObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComponent->SetCollisionProfileName(CP_MONSTER);
	SetRootComponent(CapsuleComponent);
}

void ALLL_BreakableObjectBase::ReceivePlayerAttackOrKnockBackedMonster()
{
	if (Crack < 3)
	{
		Crack++;
		UE_LOG(LogTemp, Log, TEXT("조각상 때림 : %d대"), Crack)
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("조각상 다 때림"))
	}
}

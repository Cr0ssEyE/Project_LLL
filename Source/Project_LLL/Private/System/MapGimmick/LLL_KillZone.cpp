// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_KillZone.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"

// Sets default values
ALLL_KillZone::ALLL_KillZone()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DeadZone"));
	Trigger->SetBoxExtent(FVector(5000.0f, 5000.0f, 500.0f));
	Trigger->SetCollisionProfileName(CP_OVERLAP_ALL);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ALLL_KillZone::OnKillTriggerBeginOverlap);
}

// Called when the game starts or when spawned
void ALLL_KillZone::BeginPlay()
{
	Super::BeginPlay();
	


}

void ALLL_KillZone::OnKillTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(OtherActor);
	if (IsValid(Character))
	{
		Character->Dead();
	}
} 


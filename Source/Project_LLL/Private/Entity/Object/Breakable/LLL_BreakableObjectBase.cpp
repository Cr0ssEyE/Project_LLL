// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"
#include "Util/LLLConstructorHelper.h"
#include "Entity/Character/Player/LLL_PlayerGoldComponet.h"

// Sets default values
ALLL_BreakableObjectBase::ALLL_BreakableObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	BaseMesh->SetStaticMesh(FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Statue.SM_Statue'"), EAssertionLevel::Check));
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ALLL_BreakableObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

float ALLL_BreakableObjectBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UActorComponent* ActorComponent = DamageCauser->GetComponentByClass(ULLL_PlayerGoldComponet::StaticClass());
	ULLL_PlayerGoldComponet* PlayerGoldComponent = Cast<ULLL_PlayerGoldComponet>(ActorComponent);
	
	return 0.0f;
}

// Called every frame
void ALLL_BreakableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


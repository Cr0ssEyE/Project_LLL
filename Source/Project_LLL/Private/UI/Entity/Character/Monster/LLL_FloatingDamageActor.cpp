// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Monster/LLL_FloatingDamageActor.h"

#include "Components/WidgetComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Util/LLL_ConstructorHelper.h"

FVector impulse = FVector(200.0f, 100.0f, 400.0f);
// Sets default values
ALLL_FloatingDamageActor::ALLL_FloatingDamageActor()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetVisibility(false);
	SetRootComponent(SphereComponent);
	SphereComponent->SetSimulatePhysics(true);
	
	DamageWidgetClass = FLLL_ConstructorHelper::FindAndGetClass<ULLL_DamageWidget>(PATH_UI_FLOATING_DAMAGE, EAssertionLevel::Check);
	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
	
	DamageWidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALLL_FloatingDamageActor::BeginPlay()
{
	Super::BeginPlay();

	DamageWidget = CastChecked<ULLL_DamageWidget>(CreateWidget(GetWorld(), DamageWidgetClass));

	DamageWidgetComponent->SetWidget(DamageWidget);
	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidgetComponent->SetRelativeLocation(GetActorLocation());
	DamageWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComponent->AddImpulse(impulse);
	DamageWidget->AnimationEndDelegate.AddUObject(this, &ALLL_FloatingDamageActor::DestroySelf);
}

void ALLL_FloatingDamageActor::DestroySelf()
{
	Destroy();
}

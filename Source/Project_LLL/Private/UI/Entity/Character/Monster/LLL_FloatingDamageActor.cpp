// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Monster/LLL_FloatingDamageActor.h"

#include "Components/WidgetComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Util/LLL_ConstructorHelper.h"

// Sets default values
ALLL_FloatingDamageActor::ALLL_FloatingDamageActor()
{
	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent->SetVisibility(false);
	
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
}

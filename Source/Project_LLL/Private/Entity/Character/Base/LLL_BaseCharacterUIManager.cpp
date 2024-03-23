// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacterUIManager.h"

#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GAS/Attribute/Base/LLL_CharacterAttributeSetBase.h"
#include "UI/LLL_CharacterStatusWidget.h"


// Sets default values for this component's properties
ULLL_BaseCharacterUIManager::ULLL_BaseCharacterUIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULLL_BaseCharacterUIManager::BeginPlay()
{
	Super::BeginPlay();
	
	ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(GetOwner());
	Character->TakeDamageDelegate.AddUObject(this, &ULLL_BaseCharacterUIManager::UpdateWidget);
}


// Called every frame
void ULLL_BaseCharacterUIManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULLL_BaseCharacterUIManager::UpdateWidget()
{
	const ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(GetOwner());
	const ULLL_CharacterAttributeSetBase* CharacterAttributeSet = CastChecked<ULLL_CharacterAttributeSetBase>(Character->GetAbilitySystemComponent()->GetAttributeSet(ULLL_CharacterAttributeSetBase::StaticClass()));

	// TODO: 체력 시스템 구현하고 만들기
	CharacterStatusWidget->UpdateWidgetView(CharacterAttributeSet);
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "LLL_CharacterAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULLL_CharacterAttributeSetBase();
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, CurrentHealth)
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, OffencePower);
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, MoveSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, AccelerateSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, TurnSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, GroundFriction);
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, AttackSpeed);
	
	ATTRIBUTE_ACCESSORS(ULLL_CharacterAttributeSetBase, ReceiveDamage);
	
	// 캐릭터 기본 어트리뷰트 데이터
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData OffencePower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MoveSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AccelerateSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TurnSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData GroundFriction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackSpeed;
	
	// 간접 접근을 위한 메타 어트리뷰트 데이터
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ReceiveDamage;
};

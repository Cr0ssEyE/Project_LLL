// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM()
enum class EAssertionLevel : uint8
{
	None,
	Ensure,
	Check,
};

class PROJECT_LLL_API FLLLConstructorHelper
{
public:
	template <class T>
	static T* FindAndGetObject(const TCHAR* InName, const EAssertionLevel InAssertionLevel = EAssertionLevel::None, const uint32 InLoadFlags = LOAD_None)
	{
		static TMap<FName, UObject*> Cache;
		
		if (Cache.Contains(InName))
		{
			return Cast<T>(Cache[InName]);
		}

		ConstructorHelpers::FObjectFinder<T> ObjectFinder(InName, InLoadFlags);
		AssertByAssertionLevel(ObjectFinder, InAssertionLevel);
		T* Object = ObjectFinder.Object;
		Cache.Add(InName, Object);
		return Object;
	}

	template <class T>
	static void FindObjectAndInitialize(const TCHAR* InName, TFunctionRef<void(T*)> Func, const EAssertionLevel InAssertionLevel = EAssertionLevel::None, uint32 InLoadFlags = LOAD_None)
	{
		if (T* Object = FindAndGetObject<T>(InName, InAssertionLevel, InLoadFlags))
		{
			Func(Object);
		}
	}

	template <class T>
	static TSubclassOf<T> FindAndGetClass(const TCHAR* InName, const EAssertionLevel InAssertionLevel = EAssertionLevel::None)
	{
		static TMap<FName, TSubclassOf<T>> Cache;
		
		if (Cache.Contains(InName))
		{
			return Cache[InName];
		}
		
		ConstructorHelpers::FClassFinder<T> ClassFinder(InName);
		
		AssertByAssertionLevel(ClassFinder, InAssertionLevel);
		TSubclassOf<T> Class = ClassFinder.Class;
		Cache.Add(InName, Class);
		
		return Class;
	}

	template <class T>
	static void FindClassAndInitialize(const TCHAR* InName, TFunctionRef<void(TSubclassOf<T>)> Func, const EAssertionLevel InAssertionLevel = EAssertionLevel::None)
	{
		if (TSubclassOf<T> Class = FindAndGetClass<T>(InName, InAssertionLevel))
		{
			Func(Class);
		}
	}

private:
	template <class T>
	static void AssertByAssertionLevel(T& InFinder, const EAssertionLevel InAssertionLevel)
	{
		switch (InAssertionLevel)
		{
		case EAssertionLevel::None:
			break;
		case EAssertionLevel::Ensure:
			ensure(InFinder.Succeeded());
			break;
		case EAssertionLevel::Check:
			check(InFinder.Succeeded());
			break;
		default:
			checkNoEntry();
		}
	}
};

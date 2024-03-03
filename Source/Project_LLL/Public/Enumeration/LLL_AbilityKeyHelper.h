#pragma once

#include "CoreMinimal.h"

// TMap<>과 같은 형식으로 키 값에 따라 고유한 어빌리티를 지정하는 경우 사용하기 위함
UENUM(BlueprintType)
enum class EAbilityInputName
{
	Attack,
	Skill,
	Dash
};


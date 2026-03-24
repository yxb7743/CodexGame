// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessAttributeSet.h"

void UChessAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute() || Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
	else if (Attribute == GetAttackRangeAttribute() || Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Max(0.1f, NewValue);
	}
}

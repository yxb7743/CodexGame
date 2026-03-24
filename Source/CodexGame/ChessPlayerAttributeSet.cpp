// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerAttributeSet.h"

void UChessPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute() || Attribute == GetMaxHealthAttribute() || Attribute == GetUltimateChargeAttribute() || Attribute == GetMaxUltimateChargeAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
}

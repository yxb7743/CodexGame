// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChessBoardTypes.generated.h"

USTRUCT(BlueprintType)
struct FChessBoardCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	FIntPoint Coordinates = FIntPoint::ZeroValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	bool bIsBench = true;

	bool operator==(const FChessBoardCell& Other) const
	{
		return Coordinates == Other.Coordinates && bIsBench == Other.bIsBench;
	}
};

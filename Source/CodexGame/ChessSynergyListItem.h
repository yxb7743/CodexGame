// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ChessSynergyTypes.h"
#include "ChessSynergyListItem.generated.h"

UCLASS(BlueprintType)
class CODEXGAME_API UChessSynergyListItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Synergy")
	FChessSynergyStatus Status;

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void InitializeFromStatus(const FChessSynergyStatus& InStatus);
};

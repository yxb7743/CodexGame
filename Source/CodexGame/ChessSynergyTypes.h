// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ChessSynergyTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FChessSynergyThreshold
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	int32 RequiredCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	FText TierName;
};

USTRUCT(BlueprintType)
struct FChessSynergyDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	FGameplayTag SynergyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	TArray<FChessSynergyThreshold> Thresholds;
};

USTRUCT(BlueprintType)
struct FChessSynergyStatus
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Synergy")
	FGameplayTag SynergyTag;

	UPROPERTY(BlueprintReadOnly, Category = "Synergy")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, Category = "Synergy")
	int32 CurrentCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Synergy")
	int32 ActiveThreshold = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Synergy")
	int32 NextThreshold = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Synergy")
	bool bIsActive = false;
};

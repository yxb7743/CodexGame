// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ChessPiece.generated.h"

UCLASS(Blueprintable)
class CODEXGAME_API AChessPiece : public AActor
{
	GENERATED_BODY()

public:
	AChessPiece();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	FGameplayTagContainer SynergyTags;

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	bool HasSynergyTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void AddSynergyTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void RemoveSynergyTag(FGameplayTag Tag);
};

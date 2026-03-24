// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChessSynergyTypes.h"
#include "ChessSynergyComponent.generated.h"

class AChessPiece;
class UGameplayEffect;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CODEXGAME_API UChessSynergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChessSynergyComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	TArray<FChessSynergyDefinition> SynergyDefinitions;

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void RebuildSynergies(const TArray<AChessPiece*>& BoardPieces);

	UFUNCTION(BlueprintPure, Category = "Synergy")
	TArray<FChessSynergyStatus> GetSortedSynergyStatuses() const;

	UFUNCTION(BlueprintPure, Category = "Synergy")
	int32 GetCurrentCountForTag(FGameplayTag SynergyTag) const;

private:
	int32 GetActiveThresholdForCount(const FChessSynergyDefinition& Definition, int32 Count, TSubclassOf<UGameplayEffect>& OutEffectClass) const;
	int32 GetNextThresholdForCount(const FChessSynergyDefinition& Definition, int32 Count) const;

	TArray<FChessSynergyStatus> CachedStatuses;
};

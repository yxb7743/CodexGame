// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChessSynergyPanelWidget.generated.h"

class AChessPlayerState;
class UChessSynergyListItem;

UCLASS(Abstract, Blueprintable)
class CODEXGAME_API UChessSynergyPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void BindToPlayerState(AChessPlayerState* PlayerState);

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void RefreshFromPlayerState(AChessPlayerState* PlayerState);

	UFUNCTION(BlueprintPure, Category = "Synergy")
	TArray<UChessSynergyListItem*> GetSynergyItems() const;

protected:
	UFUNCTION()
	void HandleObservedSynergiesChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "Synergy")
	void OnSynergyItemsChanged();

	UPROPERTY(BlueprintReadOnly, Category = "Synergy")
	TArray<TObjectPtr<UChessSynergyListItem>> SynergyItems;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Synergy")
	TObjectPtr<AChessPlayerState> ObservedPlayerState;
};

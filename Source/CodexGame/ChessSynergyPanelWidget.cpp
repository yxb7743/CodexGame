// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessSynergyPanelWidget.h"

#include "ChessPlayerState.h"
#include "ChessSynergyComponent.h"
#include "ChessSynergyListItem.h"

void UChessSynergyPanelWidget::BindToPlayerState(AChessPlayerState* PlayerState)
{
	if (ObservedPlayerState != nullptr)
	{
		ObservedPlayerState->OnSynergiesChanged.RemoveDynamic(this, &UChessSynergyPanelWidget::HandleObservedSynergiesChanged);
	}

	ObservedPlayerState = PlayerState;

	if (ObservedPlayerState != nullptr)
	{
		ObservedPlayerState->OnSynergiesChanged.AddDynamic(this, &UChessSynergyPanelWidget::HandleObservedSynergiesChanged);
	}

	RefreshFromPlayerState(ObservedPlayerState);
}

void UChessSynergyPanelWidget::RefreshFromPlayerState(AChessPlayerState* PlayerState)
{
	SynergyItems.Reset();

	if (PlayerState == nullptr || PlayerState->SynergyComponent == nullptr)
	{
		OnSynergyItemsChanged();
		return;
	}

	const TArray<FChessSynergyStatus> Statuses = PlayerState->SynergyComponent->GetSortedSynergyStatuses();
	for (const FChessSynergyStatus& Status : Statuses)
	{
		UChessSynergyListItem* NewItem = NewObject<UChessSynergyListItem>(this);
		NewItem->InitializeFromStatus(Status);
		SynergyItems.Add(NewItem);
	}

	OnSynergyItemsChanged();
}

void UChessSynergyPanelWidget::HandleObservedSynergiesChanged()
{
	RefreshFromPlayerState(ObservedPlayerState);
}

TArray<UChessSynergyListItem*> UChessSynergyPanelWidget::GetSynergyItems() const
{
	TArray<UChessSynergyListItem*> Result;
	Result.Reserve(SynergyItems.Num());
	for (UChessSynergyListItem* Item : SynergyItems)
	{
		Result.Add(Item);
	}

	return Result;
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessSynergyComponent.h"

#include "ChessPiece.h"
#include "GameplayEffect.h"

UChessSynergyComponent::UChessSynergyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChessSynergyComponent::RebuildSynergies(const TArray<AChessPiece*>& BoardPieces)
{
	TMap<FGameplayTag, int32> CountsByTag;
	for (AChessPiece* Piece : BoardPieces)
	{
		if (Piece == nullptr)
		{
			continue;
		}

		for (const FGameplayTag& Tag : Piece->SynergyTags)
		{
			CountsByTag.FindOrAdd(Tag)++;
		}
	}

	CachedStatuses.Reset();

	for (const FChessSynergyDefinition& Definition : SynergyDefinitions)
	{
		const int32 CurrentCount = CountsByTag.FindRef(Definition.SynergyTag);
		TSubclassOf<UGameplayEffect> EffectClass;
		const int32 ActiveThreshold = GetActiveThresholdForCount(Definition, CurrentCount, EffectClass);
		const int32 NextThreshold = GetNextThresholdForCount(Definition, CurrentCount);

		FChessSynergyStatus Status;
		Status.SynergyTag = Definition.SynergyTag;
		Status.DisplayName = Definition.DisplayName;
		Status.CurrentCount = CurrentCount;
		Status.ActiveThreshold = ActiveThreshold;
		Status.NextThreshold = NextThreshold;
		Status.bIsActive = ActiveThreshold > 0;
		CachedStatuses.Add(Status);

		for (AChessPiece* Piece : BoardPieces)
		{
			if (Piece == nullptr || !Piece->HasSynergyTag(Definition.SynergyTag))
			{
				continue;
			}

			Piece->RefreshSynergyEffect(Definition.SynergyTag, ActiveThreshold > 0 ? EffectClass : nullptr);
		}
	}

	CachedStatuses.Sort([](const FChessSynergyStatus& Left, const FChessSynergyStatus& Right)
	{
		if (Left.ActiveThreshold != Right.ActiveThreshold)
		{
			return Left.ActiveThreshold > Right.ActiveThreshold;
		}

		if (Left.bIsActive != Right.bIsActive)
		{
			return Left.bIsActive && !Right.bIsActive;
		}

		if (Left.CurrentCount != Right.CurrentCount)
		{
			return Left.CurrentCount > Right.CurrentCount;
		}

		return Left.DisplayName.ToString() < Right.DisplayName.ToString();
	});
}

TArray<FChessSynergyStatus> UChessSynergyComponent::GetSortedSynergyStatuses() const
{
	return CachedStatuses;
}

int32 UChessSynergyComponent::GetCurrentCountForTag(FGameplayTag SynergyTag) const
{
	for (const FChessSynergyStatus& Status : CachedStatuses)
	{
		if (Status.SynergyTag == SynergyTag)
		{
			return Status.CurrentCount;
		}
	}

	return 0;
}

int32 UChessSynergyComponent::GetActiveThresholdForCount(const FChessSynergyDefinition& Definition, int32 Count, TSubclassOf<UGameplayEffect>& OutEffectClass) const
{
	int32 BestThreshold = 0;
	OutEffectClass = nullptr;

	for (const FChessSynergyThreshold& Threshold : Definition.Thresholds)
	{
		if (Count >= Threshold.RequiredCount && Threshold.RequiredCount >= BestThreshold)
		{
			BestThreshold = Threshold.RequiredCount;
			OutEffectClass = Threshold.EffectClass;
		}
	}

	return BestThreshold;
}

int32 UChessSynergyComponent::GetNextThresholdForCount(const FChessSynergyDefinition& Definition, int32 Count) const
{
	int32 NextThreshold = 0;

	for (const FChessSynergyThreshold& Threshold : Definition.Thresholds)
	{
		if (Threshold.RequiredCount > Count)
		{
			if (NextThreshold == 0 || Threshold.RequiredCount < NextThreshold)
			{
				NextThreshold = Threshold.RequiredCount;
			}
		}
	}

	return NextThreshold;
}

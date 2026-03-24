// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessPiece.h"

AChessPiece::AChessPiece()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AChessPiece::HasSynergyTag(FGameplayTag Tag) const
{
	return SynergyTags.HasTag(Tag);
}

void AChessPiece::AddSynergyTag(FGameplayTag Tag)
{
	if (Tag.IsValid())
	{
		SynergyTags.AddTag(Tag);
	}
}

void AChessPiece::RemoveSynergyTag(FGameplayTag Tag)
{
	if (Tag.IsValid())
	{
		SynergyTags.RemoveTag(Tag);
	}
}

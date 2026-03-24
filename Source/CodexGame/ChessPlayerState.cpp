// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AutoChessBoard.h"
#include "ChessAbilitySystemComponent.h"
#include "ChessPiece.h"
#include "ChessPlayerAttributeSet.h"
#include "ChessPlayerUltimateAbility.h"
#include "ChessSynergyComponent.h"

AChessPlayerState::AChessPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UChessAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(false);

	PlayerAttributeSet = CreateDefaultSubobject<UChessPlayerAttributeSet>(TEXT("PlayerAttributeSet"));
	PlayerAttributeSet->InitHealth(100.0f);
	PlayerAttributeSet->InitMaxHealth(100.0f);
	PlayerAttributeSet->InitUltimateCharge(0.0f);
	PlayerAttributeSet->InitMaxUltimateCharge(100.0f);
	SynergyComponent = CreateDefaultSubobject<UChessSynergyComponent>(TEXT("SynergyComponent"));
}

UAbilitySystemComponent* AChessPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AChessPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->SetupAbilityActorInfo(this, GetPawn());
		GrantUniqueUltimateAbility();
	}
}

void AChessPlayerState::AddGold(int32 Amount)
{
	Gold = FMath::Max(0, Gold + Amount);
	OnGoldChanged.Broadcast(Gold);
}

bool AChessPlayerState::SpendGold(int32 Amount)
{
	if (!CanAfford(Amount))
	{
		return false;
	}

	Gold -= Amount;
	OnGoldChanged.Broadcast(Gold);
	return true;
}

bool AChessPlayerState::CanAfford(int32 Amount) const
{
	return Gold >= Amount;
}

void AChessPlayerState::AddExperience(int32 Amount)
{
	Experience = FMath::Max(0, Experience + Amount);

	int32 NewLevel = Level;
	while (Experience >= NewLevel * 4)
	{
		Experience -= NewLevel * 4;
		++NewLevel;
	}

	if (NewLevel != Level)
	{
		Level = NewLevel;
		OnLevelChanged.Broadcast(Level);
	}

	OnExperienceChanged.Broadcast(Experience);
}

bool AChessPlayerState::PurchaseExperience()
{
	if (!SpendGold(ExperiencePurchaseCost))
	{
		return false;
	}

	AddExperience(ExperiencePerPurchase);
	return true;
}

void AChessPlayerState::AddUltimateCharge(float Amount)
{
	if (PlayerAttributeSet == nullptr)
	{
		return;
	}

	const float NewCharge = FMath::Clamp(PlayerAttributeSet->GetUltimateCharge() + Amount, 0.0f, PlayerAttributeSet->GetMaxUltimateCharge());
	PlayerAttributeSet->SetUltimateCharge(NewCharge);
	OnUltimateChargeChanged.Broadcast(NewCharge);
}

bool AChessPlayerState::CanCastUltimate() const
{
	return PlayerAttributeSet != nullptr && PlayerAttributeSet->GetUltimateCharge() >= PlayerAttributeSet->GetMaxUltimateCharge();
}

bool AChessPlayerState::ConsumeUltimateCharge()
{
	if (!CanCastUltimate() || PlayerAttributeSet == nullptr)
	{
		return false;
	}

	PlayerAttributeSet->SetUltimateCharge(0.0f);
	OnUltimateChargeChanged.Broadcast(0.0f);
	return true;
}

bool AChessPlayerState::ActivateUniqueUltimate()
{
	if (AbilitySystemComponent == nullptr || !CanCastUltimate())
	{
		return false;
	}

	if (UniqueUltimateAbilityHandle.IsValid())
	{
		return AbilitySystemComponent->TryActivateAbility(UniqueUltimateAbilityHandle);
	}

	return false;
}

void AChessPlayerState::SetPreparationPhase(bool bNewPreparationPhase)
{
	bIsPreparationPhase = bNewPreparationPhase;
}

bool AChessPlayerState::CanRepositionPieces() const
{
	return bIsPreparationPhase;
}

void AChessPlayerState::RegisterOwnedPiece(AChessPiece* Piece, bool bStartOnBench)
{
	if (Piece == nullptr)
	{
		return;
	}

	OwnedPieces.AddUnique(Piece);
	Piece->SetOwningPlayerState(this);

	if (bStartOnBench)
	{
		BenchPieces.AddUnique(Piece);
		BoardPieces.Remove(Piece);
	}
}

void AChessPlayerState::RemoveOwnedPiece(AChessPiece* Piece)
{
	if (Piece == nullptr)
	{
		return;
	}

	if (ActiveBoard.IsValid() && Piece->HasBoardCell())
	{
		ActiveBoard->RemovePiece(Piece);
	}

	OwnedPieces.Remove(Piece);
	BenchPieces.Remove(Piece);
	BoardPieces.Remove(Piece);
	RefreshSynergies();
}

bool AChessPlayerState::MovePieceToCell(AChessPiece* Piece, AAutoChessBoard* BoardActor, const FChessBoardCell& TargetCell)
{
	if (Piece == nullptr || BoardActor == nullptr || !CanRepositionPieces())
	{
		return false;
	}

	ActiveBoard = BoardActor;

	if (!OwnedPieces.Contains(Piece))
	{
		RegisterOwnedPiece(Piece, TargetCell.bIsBench);
	}

	if (!BoardActor->TryPlacePiece(Piece, TargetCell))
	{
		return false;
	}

	if (TargetCell.bIsBench)
	{
		BenchPieces.AddUnique(Piece);
		BoardPieces.Remove(Piece);
	}
	else
	{
		BoardPieces.AddUnique(Piece);
		BenchPieces.Remove(Piece);
	}

	RefreshSynergies();
	return true;
}

void AChessPlayerState::RefreshSynergies()
{
	if (SynergyComponent != nullptr)
	{
		TArray<AChessPiece*> ValidBoardPieces;
		for (AChessPiece* Piece : BoardPieces)
		{
			if (Piece != nullptr)
			{
				ValidBoardPieces.Add(Piece);
			}
		}

		SynergyComponent->RebuildSynergies(ValidBoardPieces);
	}

	OnSynergiesChanged.Broadcast();
}

void AChessPlayerState::GrantUniqueUltimateAbility()
{
	if (AbilitySystemComponent == nullptr || UniqueUltimateAbilityClass == nullptr || UniqueUltimateAbilityHandle.IsValid())
	{
		return;
	}

	UniqueUltimateAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UniqueUltimateAbilityClass, 1, 0));
}

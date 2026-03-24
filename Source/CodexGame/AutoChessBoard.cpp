// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoChessBoard.h"

#include "ChessPiece.h"

AAutoChessBoard::AAutoChessBoard()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AAutoChessBoard::IsValidCell(const FChessBoardCell& Cell) const
{
	if (Cell.bIsBench)
	{
		return Cell.Coordinates.X >= 0 && Cell.Coordinates.X < BenchColumns;
	}

	return Cell.Coordinates.X >= 0 && Cell.Coordinates.X < BoardColumns
		&& Cell.Coordinates.Y >= 0 && Cell.Coordinates.Y < BoardRows;
}

bool AAutoChessBoard::IsCellOccupied(const FChessBoardCell& Cell) const
{
	if (!IsValidCell(Cell))
	{
		return false;
	}

	const TWeakObjectPtr<AChessPiece>* FoundPiece = OccupancyMap.Find(MakeCellKey(Cell));
	return FoundPiece != nullptr && FoundPiece->IsValid();
}

bool AAutoChessBoard::TryPlacePiece(AChessPiece* Piece, const FChessBoardCell& Cell)
{
	if (Piece == nullptr || !IsValidCell(Cell))
	{
		return false;
	}

	if (Piece->HasBoardCell())
	{
		OccupancyMap.Remove(MakeCellKey(Piece->GetBoardCell()));
	}

	if (IsCellOccupied(Cell))
	{
		return false;
	}

	Piece->SetActorLocation(GetCellWorldLocation(Cell));
	Piece->SetBoardCell(Cell);
	OccupancyMap.Add(MakeCellKey(Cell), Piece);
	return true;
}

void AAutoChessBoard::RemovePiece(AChessPiece* Piece)
{
	if (Piece == nullptr || !Piece->HasBoardCell())
	{
		return;
	}

	OccupancyMap.Remove(MakeCellKey(Piece->GetBoardCell()));
	Piece->ClearBoardCell();
}

FVector AAutoChessBoard::GetCellWorldLocation(const FChessBoardCell& Cell) const
{
	const float OriginX = -0.5f * static_cast<float>(BoardColumns - 1) * CellSize;
	const float BenchOriginX = -0.5f * static_cast<float>(BenchColumns - 1) * CellSize;
	const float LocalX = (Cell.bIsBench ? BenchOriginX : OriginX) + static_cast<float>(Cell.Coordinates.X) * CellSize;
	const float LocalY = Cell.bIsBench ? BenchOffsetY : static_cast<float>(Cell.Coordinates.Y) * CellSize;
	return GetActorTransform().TransformPosition(FVector(LocalX, LocalY, 0.0f));
}

bool AAutoChessBoard::WorldToCell(const FVector& WorldLocation, FChessBoardCell& OutCell) const
{
	const FVector LocalLocation = GetActorTransform().InverseTransformPosition(WorldLocation);
	const float OriginX = -0.5f * static_cast<float>(BoardColumns - 1) * CellSize;
	const float BenchOriginX = -0.5f * static_cast<float>(BenchColumns - 1) * CellSize;

	if (FMath::Abs(LocalLocation.Y - BenchOffsetY) <= CellSize * 0.5f)
	{
		const int32 BenchColumn = FMath::RoundToInt((LocalLocation.X - BenchOriginX) / CellSize);
		OutCell = FChessBoardCell{ FIntPoint(BenchColumn, 0), true };
		return IsValidCell(OutCell);
	}

	const int32 BoardColumn = FMath::RoundToInt((LocalLocation.X - OriginX) / CellSize);
	const int32 BoardRow = FMath::RoundToInt(LocalLocation.Y / CellSize);
	OutCell = FChessBoardCell{ FIntPoint(BoardColumn, BoardRow), false };
	return IsValidCell(OutCell);
}

FString AAutoChessBoard::MakeCellKey(const FChessBoardCell& Cell) const
{
	return FString::Printf(TEXT("%s_%d_%d"), Cell.bIsBench ? TEXT("Bench") : TEXT("Board"), Cell.Coordinates.X, Cell.Coordinates.Y);
}

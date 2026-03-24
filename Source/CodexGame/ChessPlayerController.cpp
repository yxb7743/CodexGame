// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerController.h"

#include "AutoChessBoard.h"
#include "ChessPiece.h"
#include "ChessPlayerState.h"

AChessPlayerController::AChessPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AChessPlayerController::SetControlledBoard(AAutoChessBoard* InBoard)
{
	ControlledBoard = InBoard;
}

bool AChessPlayerController::SelectPiece(AChessPiece* Piece)
{
	if (Piece == nullptr)
	{
		return false;
	}

	if (AChessPlayerState* ChessPlayerState = GetChessPlayerState())
	{
		if (!ChessPlayerState->OwnedPieces.Contains(Piece))
		{
			return false;
		}
	}

	SelectedPiece = Piece;
	return true;
}

void AChessPlayerController::ClearSelection()
{
	SelectedPiece = nullptr;
}

bool AChessPlayerController::TrySelectPieceUnderCursor()
{
	FHitResult HitResult;
	if (!GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		return false;
	}

	return SelectPiece(Cast<AChessPiece>(HitResult.GetActor()));
}

bool AChessPlayerController::TryPlaceSelectedPieceUnderCursor()
{
	if (SelectedPiece == nullptr || ControlledBoard == nullptr)
	{
		return false;
	}

	FHitResult HitResult;
	if (!GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		return false;
	}

	FChessBoardCell TargetCell;
	if (!ControlledBoard->WorldToCell(HitResult.ImpactPoint, TargetCell))
	{
		return false;
	}

	if (AChessPlayerState* ChessPlayerState = GetChessPlayerState())
	{
		return ChessPlayerState->MovePieceToCell(SelectedPiece, ControlledBoard, TargetCell);
	}

	return false;
}

bool AChessPlayerController::TryActivatePlayerUltimate()
{
	if (AChessPlayerState* ChessPlayerState = GetChessPlayerState())
	{
		return ChessPlayerState->ActivateUniqueUltimate();
	}

	return false;
}

AChessPlayerState* AChessPlayerController::GetChessPlayerState() const
{
	return GetPlayerState<AChessPlayerState>();
}

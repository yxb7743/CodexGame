// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChessPlayerController.generated.h"

class AAutoChessBoard;
class AChessPiece;
class AChessPlayerState;

UCLASS()
class CODEXGAME_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AChessPlayerController();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	TObjectPtr<AAutoChessBoard> ControlledBoard;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Selection")
	TObjectPtr<AChessPiece> SelectedPiece;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetControlledBoard(AAutoChessBoard* InBoard);

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool SelectPiece(AChessPiece* Piece);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void ClearSelection();

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool TrySelectPieceUnderCursor();

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool TryPlaceSelectedPieceUnderCursor();

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool TryActivatePlayerUltimate();

	UFUNCTION(BlueprintPure, Category = "Input")
	AChessPlayerState* GetChessPlayerState() const;
};

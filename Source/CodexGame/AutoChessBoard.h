// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChessBoardTypes.h"
#include "GameFramework/Actor.h"
#include "AutoChessBoard.generated.h"

class AChessPiece;

UCLASS(Blueprintable)
class CODEXGAME_API AAutoChessBoard : public AActor
{
	GENERATED_BODY()

public:
	AAutoChessBoard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	int32 BoardRows = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	int32 BoardColumns = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	int32 BenchColumns = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	float CellSize = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	float BenchOffsetY = -250.0f;

	UFUNCTION(BlueprintPure, Category = "Board")
	bool IsValidCell(const FChessBoardCell& Cell) const;

	UFUNCTION(BlueprintPure, Category = "Board")
	bool IsCellOccupied(const FChessBoardCell& Cell) const;

	UFUNCTION(BlueprintCallable, Category = "Board")
	bool TryPlacePiece(AChessPiece* Piece, const FChessBoardCell& Cell);

	UFUNCTION(BlueprintCallable, Category = "Board")
	void RemovePiece(AChessPiece* Piece);

	UFUNCTION(BlueprintPure, Category = "Board")
	FVector GetCellWorldLocation(const FChessBoardCell& Cell) const;

	UFUNCTION(BlueprintPure, Category = "Board")
	bool WorldToCell(const FVector& WorldLocation, FChessBoardCell& OutCell) const;

private:
	FString MakeCellKey(const FChessBoardCell& Cell) const;

	TMap<FString, TWeakObjectPtr<AChessPiece>> OccupancyMap;
};

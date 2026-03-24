// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BattleSystem.generated.h"

USTRUCT(BlueprintType)
struct FChessPieceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	FString Name = TEXT("Piece");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (ClampMin = "1"))
	int32 MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (ClampMin = "0"))
	int32 CurrentHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (ClampMin = "0"))
	int32 Attack = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (ClampMin = "0"))
	int32 Defense = 5;
};

USTRUCT(BlueprintType)
struct FBattleTurnResult
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	int32 DamageDealt = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	int32 DefenderHealthAfterHit = 0;
};

USTRUCT(BlueprintType)
struct FBattleResult
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	FChessPieceData Winner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	FChessPieceData Loser;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	int32 TotalTurns = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
	TArray<FString> CombatLog;
};

UCLASS()
class CODEXGAME_API UBattleSystem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Battle")
	static FBattleTurnResult ExecuteTurn(UPARAM(ref) FChessPieceData& Attacker, UPARAM(ref) FChessPieceData& Defender);

	UFUNCTION(BlueprintCallable, Category = "Battle")
	static FBattleResult SimulateBattle(FChessPieceData PieceA, FChessPieceData PieceB);

	UFUNCTION(BlueprintCallable, Category = "Battle")
	static bool IsAlive(const FChessPieceData& Piece);
};

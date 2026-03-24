// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleSystem.h"
#include "BattleTestActor.generated.h"

UCLASS(Blueprintable)
class CODEXGAME_API ABattleTestActor : public AActor
{
	GENERATED_BODY()

public:
	ABattleTestActor();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Test")
	FChessPieceData PieceA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Test")
	FChessPieceData PieceB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Test")
	bool bAutoRunOnBeginPlay = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Test")
	FBattleResult LastBattleResult;

	UFUNCTION(BlueprintCallable, Category = "Battle Test")
	FBattleResult RunBattleTest();
};

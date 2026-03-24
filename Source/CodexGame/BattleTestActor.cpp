// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattleTestActor.h"

#include "Engine/Engine.h"

ABattleTestActor::ABattleTestActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PieceA.Name = TEXT("Knight");
	PieceA.MaxHealth = 100;
	PieceA.CurrentHealth = 100;
	PieceA.Attack = 30;
	PieceA.Defense = 10;

	PieceB.Name = TEXT("Bishop");
	PieceB.MaxHealth = 90;
	PieceB.CurrentHealth = 90;
	PieceB.Attack = 24;
	PieceB.Defense = 8;
}

void ABattleTestActor::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoRunOnBeginPlay)
	{
		RunBattleTest();
	}
}

FBattleResult ABattleTestActor::RunBattleTest()
{
	LastBattleResult = UBattleSystem::SimulateBattle(PieceA, PieceB);

	const FString WinnerMessage = FString::Printf(
		TEXT("[BattleTest] Winner: %s (HP Remaining: %d)"),
		*LastBattleResult.Winner.Name,
		LastBattleResult.Winner.CurrentHealth);

	UE_LOG(LogTemp, Display, TEXT("%s"), *WinnerMessage);

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, WinnerMessage);
	}

	for (const FString& LogLine : LastBattleResult.CombatLog)
	{
		UE_LOG(LogTemp, Display, TEXT("[BattleTest] %s"), *LogLine);
	}

	return LastBattleResult;
}

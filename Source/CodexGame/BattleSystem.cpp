// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattleSystem.h"

bool UBattleSystem::IsAlive(const FChessPieceData& Piece)
{
	return Piece.CurrentHealth > 0;
}

FBattleTurnResult UBattleSystem::ExecuteTurn(FChessPieceData& Attacker, FChessPieceData& Defender)
{
	Attacker.MaxHealth = FMath::Max(1, Attacker.MaxHealth);
	Attacker.CurrentHealth = FMath::Clamp(Attacker.CurrentHealth, 0, Attacker.MaxHealth);
	Attacker.Attack = FMath::Max(0, Attacker.Attack);
	Attacker.Defense = FMath::Max(0, Attacker.Defense);

	Defender.MaxHealth = FMath::Max(1, Defender.MaxHealth);
	Defender.CurrentHealth = FMath::Clamp(Defender.CurrentHealth, 0, Defender.MaxHealth);
	Defender.Attack = FMath::Max(0, Defender.Attack);
	Defender.Defense = FMath::Max(0, Defender.Defense);

	const int32 Damage = FMath::Max(1, Attacker.Attack - Defender.Defense);
	Defender.CurrentHealth = FMath::Max(0, Defender.CurrentHealth - Damage);

	FBattleTurnResult TurnResult;
	TurnResult.DamageDealt = Damage;
	TurnResult.DefenderHealthAfterHit = Defender.CurrentHealth;
	return TurnResult;
}

FBattleResult UBattleSystem::SimulateBattle(FChessPieceData PieceA, FChessPieceData PieceB)
{
	FBattleResult Result;

	PieceA.MaxHealth = FMath::Max(1, PieceA.MaxHealth);
	PieceA.CurrentHealth = FMath::Clamp(PieceA.CurrentHealth, 0, PieceA.MaxHealth);
	PieceA.Attack = FMath::Max(0, PieceA.Attack);
	PieceA.Defense = FMath::Max(0, PieceA.Defense);

	PieceB.MaxHealth = FMath::Max(1, PieceB.MaxHealth);
	PieceB.CurrentHealth = FMath::Clamp(PieceB.CurrentHealth, 0, PieceB.MaxHealth);
	PieceB.Attack = FMath::Max(0, PieceB.Attack);
	PieceB.Defense = FMath::Max(0, PieceB.Defense);

	int32 Turn = 1;
	bool bAIsAttacker = true;

	while (IsAlive(PieceA) && IsAlive(PieceB))
	{
		FChessPieceData& Attacker = bAIsAttacker ? PieceA : PieceB;
		FChessPieceData& Defender = bAIsAttacker ? PieceB : PieceA;

		const FBattleTurnResult TurnResult = ExecuteTurn(Attacker, Defender);

		Result.CombatLog.Add(FString::Printf(
			TEXT("Turn %d: %s hits %s for %d damage. %s HP: %d"),
			Turn,
			*Attacker.Name,
			*Defender.Name,
			TurnResult.DamageDealt,
			*Defender.Name,
			TurnResult.DefenderHealthAfterHit));

		++Turn;
		bAIsAttacker = !bAIsAttacker;
	}

	Result.TotalTurns = Turn - 1;

	if (IsAlive(PieceA))
	{
		Result.Winner = PieceA;
		Result.Loser = PieceB;
	}
	else
	{
		Result.Winner = PieceB;
		Result.Loser = PieceA;
	}

	Result.CombatLog.Add(FString::Printf(
		TEXT("Winner: %s (HP Remaining: %d)"),
		*Result.Winner.Name,
		Result.Winner.CurrentHealth));

	return Result;
}

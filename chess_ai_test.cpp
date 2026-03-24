// Unreal Engine automation test for chess piece attack behavior.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "BattleSystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FChessPieceAIAttackBehaviorTest,
    "CodexGame.ChessAI.AttackBehavior",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FChessPieceAIAttackBehaviorTest::RunTest(const FString& Parameters)
{
    // Assumption: AI attack behavior uses UBattleSystem::ExecuteTurn damage logic.
    FChessPieceData Attacker;
    Attacker.Name = TEXT("AI_Pawn");
    Attacker.MaxHealth = 100;
    Attacker.CurrentHealth = 100;
    Attacker.Attack = 30;
    Attacker.Defense = 5;

    FChessPieceData Defender;
    Defender.Name = TEXT("Target");
    Defender.MaxHealth = 100;
    Defender.CurrentHealth = 100;
    Defender.Attack = 10;
    Defender.Defense = 12;

    const FBattleTurnResult TurnResult = UBattleSystem::ExecuteTurn(Attacker, Defender);

    TestEqual(TEXT("Damage should be Attack - Defense when positive"), TurnResult.DamageDealt, 18);
    TestEqual(TEXT("Defender HP should decrease by computed damage"), Defender.CurrentHealth, 82);

    FChessPieceData LowAttackAttacker;
    LowAttackAttacker.Attack = 3;

    FChessPieceData HighDefenseDefender;
    HighDefenseDefender.MaxHealth = 50;
    HighDefenseDefender.CurrentHealth = 50;
    HighDefenseDefender.Defense = 20;

    const FBattleTurnResult MinDamageResult = UBattleSystem::ExecuteTurn(LowAttackAttacker, HighDefenseDefender);

    TestEqual(TEXT("Damage should have minimum of 1"), MinDamageResult.DamageDealt, 1);
    TestEqual(TEXT("Defender HP should be reduced by minimum damage"), HighDefenseDefender.CurrentHealth, 49);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChessCombatComponent.generated.h"

class AChessPiece;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CODEXGAME_API UChessCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChessCombatComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TargetSearchInterval = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AcceptanceRangeBuffer = 20.0f;

	UFUNCTION(BlueprintPure, Category = "Combat")
	AChessPiece* GetCurrentTarget() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ClearCurrentTarget();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ForceRetarget();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void OnPerformedBasicAttack(AChessPiece* TargetPiece);
	virtual void OnPerformedBasicAttack_Implementation(AChessPiece* TargetPiece);

private:
	bool CanRunCombat() const;
	AChessPiece* FindBestTarget() const;
	void MoveTowardTarget(float DeltaTime);

	TWeakObjectPtr<AChessPiece> OwnerPiece;
	TWeakObjectPtr<AChessPiece> CurrentTarget;
	float TimeUntilNextAttack = 0.0f;
	float TimeUntilNextSearch = 0.0f;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ChessBoardTypes.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ChessPiece.generated.h"

class AChessPlayerState;
class UChessAbilitySystemComponent;
class UChessAttributeSet;
class UChessCombatComponent;
class UGameplayEffect;
struct FActiveGameplayEffectHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChessPieceKillSignature, AChessPiece*, VictimPiece);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChessPieceUltimateSignature, float, AddedCharge);

UCLASS(Blueprintable)
class CODEXGAME_API AChessPiece : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AChessPiece();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UChessAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UChessAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UChessCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Synergy")
	FGameplayTagContainer SynergyTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 StarLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 PieceExperience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 ExperienceToTwoStar = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 ExperienceToThreeStar = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
	int32 KillRewardGold = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
	int32 KillRewardExperience = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float UltimateChargeGrantedToPlayer = 15.0f;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FChessPieceKillSignature OnPieceScoredKill;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FChessPieceUltimateSignature OnUltimateCast;

	UFUNCTION(BlueprintCallable, Category = "Owner")
	void SetOwningPlayerState(AChessPlayerState* NewOwner);

	UFUNCTION(BlueprintPure, Category = "Owner")
	AChessPlayerState* GetOwningPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Board")
	void SetBoardCell(const FChessBoardCell& NewCell);

	UFUNCTION(BlueprintCallable, Category = "Board")
	void ClearBoardCell();

	UFUNCTION(BlueprintPure, Category = "Board")
	bool HasBoardCell() const;

	UFUNCTION(BlueprintPure, Category = "Board")
	FChessBoardCell GetBoardCell() const;

	UFUNCTION(BlueprintPure, Category = "Synergy")
	bool HasSynergyTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void AddSynergyTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void RemoveSynergyTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Synergy")
	void RefreshSynergyEffect(FGameplayTag SynergyTag, TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanAttackTarget(const AChessPiece* TargetPiece) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformBasicAttack(AChessPiece* TargetPiece);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ReceiveDamage(float DamageAmount, AChessPiece* DamageInstigator);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void NotifyUltimateCast();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HandleDeath(AChessPiece* KillingPiece);

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void AddPieceExperience(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Progression")
	bool CanUpgradeStar() const;

	UFUNCTION(BlueprintCallable, Category = "Progression")
	bool TryUpgradeStar();

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsAlive() const;

protected:
	TWeakObjectPtr<AChessPlayerState> OwningPlayerState;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Board")
	FChessBoardCell OccupiedCell;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Board")
	bool bHasAssignedCell = false;

	TMap<FGameplayTag, FActiveGameplayEffectHandle> ActiveSynergyHandles;
};

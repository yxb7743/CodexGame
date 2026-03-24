// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ChessBoardTypes.h"
#include "GameFramework/PlayerState.h"
#include "ChessPlayerState.generated.h"

class AAutoChessBoard;
class AChessPiece;
class UChessAbilitySystemComponent;
class UChessPlayerAttributeSet;
class UChessPlayerUltimateAbility;
class UChessSynergyComponent;
struct FGameplayAbilitySpecHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerResourceChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerUltimateChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerSynergiesChangedSignature);

UCLASS()
class CODEXGAME_API AChessPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AChessPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UChessAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UChessPlayerAttributeSet> PlayerAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Synergy")
	TObjectPtr<UChessSynergyComponent> SynergyComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Gold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Experience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 ExperiencePurchaseCost = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 ExperiencePerPurchase = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool bIsPreparationPhase = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	TSubclassOf<UChessPlayerUltimateAbility> UniqueUltimateAbilityClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pieces")
	TArray<TObjectPtr<AChessPiece>> OwnedPieces;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pieces")
	TArray<TObjectPtr<AChessPiece>> BenchPieces;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pieces")
	TArray<TObjectPtr<AChessPiece>> BoardPieces;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPlayerResourceChangedSignature OnGoldChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPlayerResourceChangedSignature OnLevelChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPlayerResourceChangedSignature OnExperienceChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPlayerUltimateChangedSignature OnUltimateChargeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPlayerSynergiesChangedSignature OnSynergiesChanged;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void AddGold(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Player")
	bool SpendGold(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Player")
	bool CanAfford(int32 Amount) const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void AddExperience(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Player")
	bool PurchaseExperience();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void AddUltimateCharge(float Amount);

	UFUNCTION(BlueprintPure, Category = "Player")
	bool CanCastUltimate() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	bool ConsumeUltimateCharge();

	UFUNCTION(BlueprintCallable, Category = "Player")
	bool ActivateUniqueUltimate();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPreparationPhase(bool bNewPreparationPhase);

	UFUNCTION(BlueprintPure, Category = "Player")
	bool CanRepositionPieces() const;

	UFUNCTION(BlueprintCallable, Category = "Pieces")
	void RegisterOwnedPiece(AChessPiece* Piece, bool bStartOnBench = true);

	UFUNCTION(BlueprintCallable, Category = "Pieces")
	void RemoveOwnedPiece(AChessPiece* Piece);

	UFUNCTION(BlueprintCallable, Category = "Pieces")
	bool MovePieceToCell(AChessPiece* Piece, AAutoChessBoard* BoardActor, const FChessBoardCell& TargetCell);

	UFUNCTION(BlueprintCallable, Category = "Pieces")
	void RefreshSynergies();

protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<AAutoChessBoard> ActiveBoard;

	FGameplayAbilitySpecHandle UniqueUltimateAbilityHandle;

	void GrantUniqueUltimateAbility();
};

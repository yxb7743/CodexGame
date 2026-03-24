// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessPiece.h"

#include "AbilitySystemComponent.h"
#include "ChessAbilitySystemComponent.h"
#include "ChessAttributeSet.h"
#include "ChessCombatComponent.h"
#include "ChessPlayerState.h"
#include "GameplayEffect.h"

AChessPiece::AChessPiece()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UChessAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(false);

	AttributeSet = CreateDefaultSubobject<UChessAttributeSet>(TEXT("AttributeSet"));
	CombatComponent = CreateDefaultSubobject<UChessCombatComponent>(TEXT("CombatComponent"));
	AttributeSet->InitHealth(100.0f);
	AttributeSet->InitMaxHealth(100.0f);
	AttributeSet->InitAttackPower(20.0f);
	AttributeSet->InitDefensePower(5.0f);
	AttributeSet->InitAttackRange(250.0f);
	AttributeSet->InitAttackSpeed(1.0f);
}

UAbilitySystemComponent* AChessPiece::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AChessPiece::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->SetupAbilityActorInfo(this, this);
	}
}

void AChessPiece::SetOwningPlayerState(AChessPlayerState* NewOwner)
{
	OwningPlayerState = NewOwner;
}

AChessPlayerState* AChessPiece::GetOwningPlayerState() const
{
	return OwningPlayerState.Get();
}

void AChessPiece::SetBoardCell(const FChessBoardCell& NewCell)
{
	OccupiedCell = NewCell;
	bHasAssignedCell = true;
}

void AChessPiece::ClearBoardCell()
{
	OccupiedCell = FChessBoardCell();
	bHasAssignedCell = false;
}

bool AChessPiece::HasBoardCell() const
{
	return bHasAssignedCell;
}

FChessBoardCell AChessPiece::GetBoardCell() const
{
	return OccupiedCell;
}

bool AChessPiece::HasSynergyTag(FGameplayTag Tag) const
{
	return SynergyTags.HasTag(Tag);
}

void AChessPiece::AddSynergyTag(FGameplayTag Tag)
{
	if (Tag.IsValid())
	{
		SynergyTags.AddTag(Tag);
	}
}

void AChessPiece::RemoveSynergyTag(FGameplayTag Tag)
{
	if (Tag.IsValid())
	{
		SynergyTags.RemoveTag(Tag);
	}
}

void AChessPiece::RefreshSynergyEffect(FGameplayTag SynergyTag, TSubclassOf<UGameplayEffect> EffectClass)
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	if (FActiveGameplayEffectHandle* ExistingHandle = ActiveSynergyHandles.Find(SynergyTag))
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(*ExistingHandle);
		ActiveSynergyHandles.Remove(SynergyTag);
	}

	if (EffectClass != nullptr)
	{
		const FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
		if (EffectSpec.IsValid())
		{
			const FActiveGameplayEffectHandle NewHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
			ActiveSynergyHandles.Add(SynergyTag, NewHandle);
		}
	}
}

bool AChessPiece::CanAttackTarget(const AChessPiece* TargetPiece) const
{
	if (TargetPiece == nullptr || TargetPiece == this || AttributeSet == nullptr || !IsAlive() || !TargetPiece->IsAlive())
	{
		return false;
	}

	const float Distance = FVector::Dist2D(GetActorLocation(), TargetPiece->GetActorLocation());
	return Distance <= AttributeSet->GetAttackRange();
}

void AChessPiece::PerformBasicAttack(AChessPiece* TargetPiece)
{
	if (!CanAttackTarget(TargetPiece) || AttributeSet == nullptr)
	{
		return;
	}

	TargetPiece->ReceiveDamage(AttributeSet->GetAttackPower(), this);
}

void AChessPiece::ReceiveDamage(float DamageAmount, AChessPiece* DamageInstigator)
{
	if (AttributeSet == nullptr)
	{
		return;
	}

	const float MitigatedDamage = FMath::Max(1.0f, DamageAmount - AttributeSet->GetDefensePower());
	const float NewHealth = FMath::Max(0.0f, AttributeSet->GetHealth() - MitigatedDamage);
	AttributeSet->SetHealth(NewHealth);

	if (NewHealth <= 0.0f && DamageInstigator != nullptr)
	{
		HandleDeath(DamageInstigator);
	}
}

void AChessPiece::NotifyUltimateCast()
{
	OnUltimateCast.Broadcast(UltimateChargeGrantedToPlayer);

	if (AChessPlayerState* OwnerState = GetOwningPlayerState())
	{
		OwnerState->AddUltimateCharge(UltimateChargeGrantedToPlayer);
	}
}

void AChessPiece::AddPieceExperience(int32 Amount)
{
	PieceExperience = FMath::Max(0, PieceExperience + Amount);
	TryUpgradeStar();
}

bool AChessPiece::CanUpgradeStar() const
{
	if (StarLevel >= 3)
	{
		return false;
	}

	if (StarLevel == 1)
	{
		return PieceExperience >= ExperienceToTwoStar;
	}

	return PieceExperience >= ExperienceToThreeStar;
}

bool AChessPiece::TryUpgradeStar()
{
	if (!CanUpgradeStar())
	{
		return false;
	}

	if (StarLevel == 1)
	{
		StarLevel = 2;
	}
	else if (StarLevel == 2)
	{
		StarLevel = 3;
	}

	return true;
}

void AChessPiece::HandleDeath(AChessPiece* KillingPiece)
{
	if (KillingPiece != nullptr)
	{
		KillingPiece->OnPieceScoredKill.Broadcast(this);
		KillingPiece->AddPieceExperience(KillingPiece->KillRewardExperience);

		if (AChessPlayerState* KillerOwnerState = KillingPiece->GetOwningPlayerState())
		{
			KillerOwnerState->AddGold(KillingPiece->KillRewardGold);
		}
	}

	if (AChessPlayerState* OwnerState = GetOwningPlayerState())
	{
		OwnerState->RemoveOwnedPiece(this);
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetCanBeDamaged(false);
}

bool AChessPiece::IsAlive() const
{
	return AttributeSet != nullptr && AttributeSet->GetHealth() > 0.0f && !IsHidden();
}

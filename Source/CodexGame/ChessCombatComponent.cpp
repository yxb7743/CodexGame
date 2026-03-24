// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessCombatComponent.h"

#include "ChessAttributeSet.h"
#include "ChessPiece.h"
#include "ChessPlayerState.h"
#include "EngineUtils.h"

UChessCombatComponent::UChessCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UChessCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerPiece = Cast<AChessPiece>(GetOwner());
}

void UChessCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeUntilNextAttack = FMath::Max(0.0f, TimeUntilNextAttack - DeltaTime);
	TimeUntilNextSearch = FMath::Max(0.0f, TimeUntilNextSearch - DeltaTime);

	if (!CanRunCombat())
	{
		CurrentTarget.Reset();
		return;
	}

	if (!CurrentTarget.IsValid() || TimeUntilNextSearch <= 0.0f)
	{
		CurrentTarget = FindBestTarget();
		TimeUntilNextSearch = TargetSearchInterval;
	}

	if (!CurrentTarget.IsValid())
	{
		return;
	}

	AChessPiece* OwnerPiecePtr = OwnerPiece.Get();
	AChessPiece* TargetPiece = CurrentTarget.Get();
	if (OwnerPiecePtr == nullptr || TargetPiece == nullptr)
	{
		return;
	}

	if (OwnerPiecePtr->CanAttackTarget(TargetPiece))
	{
		if (TimeUntilNextAttack <= 0.0f)
		{
			OwnerPiecePtr->PerformBasicAttack(TargetPiece);
			OnPerformedBasicAttack(TargetPiece);
			TimeUntilNextAttack = AttackInterval;
		}
	}
	else
	{
		MoveTowardTarget(DeltaTime);
	}
}

AChessPiece* UChessCombatComponent::GetCurrentTarget() const
{
	return CurrentTarget.Get();
}

void UChessCombatComponent::ClearCurrentTarget()
{
	CurrentTarget.Reset();
}

void UChessCombatComponent::ForceRetarget()
{
	TimeUntilNextSearch = 0.0f;
}

void UChessCombatComponent::OnPerformedBasicAttack_Implementation(AChessPiece* TargetPiece)
{
}

bool UChessCombatComponent::CanRunCombat() const
{
	const AChessPiece* OwnerPiecePtr = OwnerPiece.Get();
	if (OwnerPiecePtr == nullptr || !OwnerPiecePtr->HasBoardCell() || OwnerPiecePtr->GetBoardCell().bIsBench)
	{
		return false;
	}

	const AChessPlayerState* OwnerState = OwnerPiecePtr->GetOwningPlayerState();
	return OwnerState != nullptr && !OwnerState->CanRepositionPieces();
}

AChessPiece* UChessCombatComponent::FindBestTarget() const
{
	const AChessPiece* OwnerPiecePtr = OwnerPiece.Get();
	if (OwnerPiecePtr == nullptr || GetWorld() == nullptr)
	{
		return nullptr;
	}

	AChessPiece* BestTarget = nullptr;
	float BestDistanceSq = TNumericLimits<float>::Max();

	for (TActorIterator<AChessPiece> It(GetWorld()); It; ++It)
	{
		AChessPiece* Candidate = *It;
		if (Candidate == nullptr || Candidate == OwnerPiecePtr || !Candidate->CanBeDamaged())
		{
			continue;
		}

		if (Candidate->GetOwningPlayerState() == OwnerPiecePtr->GetOwningPlayerState())
		{
			continue;
		}

		if (!Candidate->HasBoardCell() || Candidate->GetBoardCell().bIsBench)
		{
			continue;
		}

		const float DistanceSq = FVector::DistSquared2D(OwnerPiecePtr->GetActorLocation(), Candidate->GetActorLocation());
		if (DistanceSq < BestDistanceSq)
		{
			BestDistanceSq = DistanceSq;
			BestTarget = Candidate;
		}
	}

	return BestTarget;
}

void UChessCombatComponent::MoveTowardTarget(float DeltaTime)
{
	AChessPiece* OwnerPiecePtr = OwnerPiece.Get();
	AChessPiece* TargetPiece = CurrentTarget.Get();
	if (OwnerPiecePtr == nullptr || TargetPiece == nullptr || OwnerPiecePtr->AttributeSet == nullptr)
	{
		return;
	}

	const FVector OwnerLocation = OwnerPiecePtr->GetActorLocation();
	const FVector TargetLocation = TargetPiece->GetActorLocation();
	const FVector Direction = (TargetLocation - OwnerLocation).GetSafeNormal2D();
	const float DesiredRange = OwnerPiecePtr->AttributeSet->GetAttackRange() - AcceptanceRangeBuffer;

	if (FVector::Dist2D(OwnerLocation, TargetLocation) <= DesiredRange)
	{
		return;
	}

	const FVector NewLocation = OwnerLocation + Direction * MoveSpeed * DeltaTime;
	OwnerPiecePtr->SetActorLocation(FVector(NewLocation.X, NewLocation.Y, OwnerLocation.Z));
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChessPlayerUltimateAbility.h"

#include "ChessPlayerState.h"

UChessPlayerUltimateAbility::UChessPlayerUltimateAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UChessPlayerUltimateAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AChessPlayerState* ChessPlayerState = ActorInfo != nullptr ? Cast<AChessPlayerState>(ActorInfo->OwnerActor.Get()) : nullptr;
	return ChessPlayerState != nullptr && ChessPlayerState->CanCastUltimate();
}

void UChessPlayerUltimateAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AChessPlayerState* ChessPlayerState = ActorInfo != nullptr ? Cast<AChessPlayerState>(ActorInfo->OwnerActor.Get()) : nullptr;
	if (ChessPlayerState == nullptr || !ChessPlayerState->ConsumeUltimateCharge())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	OnUltimateActivated(ActorInfo != nullptr ? ActorInfo->AvatarActor.Get() : nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

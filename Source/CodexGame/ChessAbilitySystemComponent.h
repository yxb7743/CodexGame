// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ChessAbilitySystemComponent.generated.h"

UCLASS()
class CODEXGAME_API UChessAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SetupAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);
};

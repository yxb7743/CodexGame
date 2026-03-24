// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ChessPlayerAttributeSet.generated.h"

#define PLAYER_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class CODEXGAME_API UChessPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	FGameplayAttributeData Health;
	PLAYER_ATTRIBUTE_ACCESSORS(UChessPlayerAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	FGameplayAttributeData MaxHealth;
	PLAYER_ATTRIBUTE_ACCESSORS(UChessPlayerAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	FGameplayAttributeData UltimateCharge;
	PLAYER_ATTRIBUTE_ACCESSORS(UChessPlayerAttributeSet, UltimateCharge)

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	FGameplayAttributeData MaxUltimateCharge;
	PLAYER_ATTRIBUTE_ACCESSORS(UChessPlayerAttributeSet, MaxUltimateCharge)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};

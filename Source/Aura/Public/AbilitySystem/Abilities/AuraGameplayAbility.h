// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level, FText Title = FText(),FText Description = FText());
	virtual FString GetNextLevelDescription(int32 Level, FText Title = FText(),  FText Description = FText());
	static FString GetLockedDescription(int32 Level, FText Title = FText(), FText Description = FText());

protected:

	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldownCost(float InLevel = 1.f) const;
};

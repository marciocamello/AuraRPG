﻿// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level, FText Title, FText Description) override;
	virtual FString GetNextLevelDescription(int32 Level, FText Title, FText Description) override;

	UFUNCTION(BlueprintCallable, Category = "Firebolt")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FTaggedMontage& AttackMontage, bool bOverridePitch , float PitchOverride, AActor* HomingTarget);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Firebolt")
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Firebolt")
	int32 MaxNumProjectiles = 5;
};

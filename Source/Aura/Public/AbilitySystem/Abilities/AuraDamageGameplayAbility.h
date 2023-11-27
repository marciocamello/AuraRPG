// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffDuration = 5.f; 

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType) const;
};

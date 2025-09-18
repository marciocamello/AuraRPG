// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraPassiveAbility.h"
#include "AuraHaloOfProtection.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraHaloOfProtection : public UAuraPassiveAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level, FText Title, FText Description) override;
	virtual FString GetNextLevelDescription(int32 Level, FText Title, FText Description) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat PassiveAbilityCurve;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility, bool bWasCancelled) override;
};

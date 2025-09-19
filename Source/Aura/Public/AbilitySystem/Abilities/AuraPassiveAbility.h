// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraPassiveAbility.generated.h"

UENUM(BlueprintType)
enum class EPassiveAbilityType : uint8
{
	None UMETA(DisplayName = "None"),
	Protection UMETA(DisplayName = "Protection"),
	LifeSteal UMETA(DisplayName = "Life Steal"),
	ManaSteal UMETA(DisplayName = "Mana Steal")
};

/**
 *
 */
UCLASS()
class AURA_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat PassiveAbilityCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Passive Type")
	EPassiveAbilityType PassiveType = EPassiveAbilityType::None;

	virtual FString GetBaseTemplate(int32 Level, FText Title, FText Description);
	virtual FString GetSpecificEffectLine(int32 Level) PURE_VIRTUAL(UAuraPassiveAbility::GetSpecificEffectLine, return "";);

	virtual FString GetDescription(int32 Level, FText Title = FText(), FText Description = FText()) override;
	virtual FString GetNextLevelDescription(int32 Level, FText Title, FText Description) override;

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData) override;

	void ReceiveDeactivate(const FGameplayTag &AbilityTag);
};

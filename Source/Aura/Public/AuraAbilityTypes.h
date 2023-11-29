#pragma once

#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"

#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAuraDamageGameplayEffect
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DebuffDuration = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DeathImpulseMagnitude = 60.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FVector DeathImpulse = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;
	
	UPROPERTY()
	TMap<FGameplayTag, FAuraDamageGameplayEffect> DamageType;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	FScalableFloat GetDebuffDamage() const { return DebuffDamage; }
	FScalableFloat GetDebuffDuration() const { return DebuffDuration; }
	FScalableFloat GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsSuccessfulDebuff(bool bInIsSuccessFulDebuff) { bIsSuccessfulDebuff = bInIsSuccessFulDebuff; }
	void SetDebuffDamage(FScalableFloat InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDebuffDuration(FScalableFloat InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequency(FScalableFloat InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(FVector InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
	
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	FScalableFloat DebuffDamage = 0.f;

	UPROPERTY()
	FScalableFloat DebuffDuration = 0.f;

	UPROPERTY()
	FScalableFloat DebuffFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
// Copyright Axchemy Games

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Game/Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Widget Controller")
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& WidgetControllerParams, AAuraHUD*& OutAuraHUD);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Widget Controller", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Widget Controller", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Widget Controller", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Character Class Defaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Character Class Defaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Character Class Defaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Character Class Defaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static FScalableFloat GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static FScalableFloat GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static FScalableFloat GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Effects")
	static FVector GetKnockBackForce(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FScalableFloat InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FScalableFloat InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FScalableFloat InDebuffFrequency);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag InDamageType);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InDeathImpulse);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
	static void SetKnockBackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InForce);
	
	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Mechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category= "Aura Ability System Library|Gameplay Mechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Damage Effect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Mechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
	
	UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Mechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};

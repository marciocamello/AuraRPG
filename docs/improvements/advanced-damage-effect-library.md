# Advanced Damage Effect Library

## Overview

This improvement extends the `AuraAbilitySystemLibrary` with a sophisticated damage effect application system that supports the multi-damage type architecture and provides centralized damage processing.

## What Was Improved

### Enhanced ApplyDamageEffect Method

The core improvement is the `ApplyDamageEffect` method that handles complex damage scenarios:

```cpp
FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
    const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
    const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

    FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
    EffectContextHandle.AddSourceObject(SourceAvatarActor);

    // Process each damage type individually
    for(const TTuple<FGameplayTag, FAuraDamageGameplayEffect>& Pair : DamageEffectParams.DamageType)
    {
        const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
            Pair.Value.DamageEffectClass,
            DamageEffectParams.AbilityLevel,
            EffectContextHandle
        );

        // Apply scaled damage
        const float ScaledDamage = Pair.Value.Damage.GetValueAtLevel(DamageEffectParams.AbilityLevel);
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);

        // Set impulse forces
        SetDeathImpulse(EffectContextHandle, Pair.Value.DeathImpulse);
        SetKnockBackForce(EffectContextHandle, Pair.Value.KnockBackForce);

        // Configure debuff parameters
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
            SpecHandle,
            GameplayTags.Debuff_Chance,
            Pair.Value.DebuffChance.GetValueAtLevel(DamageEffectParams.AbilityLevel)
        );

        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
            SpecHandle,
            GameplayTags.Debuff_Damage,
            Pair.Value.DebuffDamage.GetValueAtLevel(DamageEffectParams.AbilityLevel)
        );

        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
            SpecHandle,
            GameplayTags.Debuff_Frequency,
            Pair.Value.DebuffFrequency.GetValueAtLevel(DamageEffectParams.AbilityLevel)
        );

        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
            SpecHandle,
            GameplayTags.Debuff_Duration,
            Pair.Value.DebuffDuration.GetValueAtLevel(DamageEffectParams.AbilityLevel)
        );

        // Set damage type for context
        SetDamageType(EffectContextHandle, Pair.Key);

        // Apply the effect
        FActiveGameplayEffectHandle ActiveEffectHandle = DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

    return EffectContextHandle;
}
```

### Enhanced Context Setters

New methods for setting complex damage context data:

```cpp
// Set impulse forces
void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, FVector InDeathImpulse)
{
    if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetDeathImpulse(InDeathImpulse);
    }
}

void UAuraAbilitySystemLibrary::SetKnockBackForce(FGameplayEffectContextHandle& EffectContextHandle, FVector InForce)
{
    if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetKnockBackForce(InForce);
    }
}

// Set debuff parameters
void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, FScalableFloat InDebuffDamage)
{
    if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetDebuffDamage(InDebuffDamage);
    }
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag InDamageType)
{
    if(FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
        AuraEffectContext->SetDamageType(DamageType);
    }
}
```

## Implementation Details

### Centralized Damage Processing

The library provides a single point for applying complex damage effects:

```cpp
// Usage in abilities
FDamageEffectParams DamageParams = MakeDamageEffectParamsFromClassDefaults(TargetActor);
UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageParams);
```

### Multi-Effect Application

Each damage type in the map gets its own effect application:

1. **Individual Effect Specs**: Each damage type creates its own `FGameplayEffectSpecHandle`
2. **Unique Configuration**: Each effect can have different classes, magnitudes, and parameters
3. **Proper Context**: Each effect gets proper context with damage type, impulses, and debuff data
4. **Network Sync**: All effects properly replicate across network

### Enhanced Context System

The `FAuraGameplayEffectContext` supports complex data:

```cpp
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
    // Damage classification
    bool bIsBlockedHit = false;
    bool bIsCriticalHit = false;
    bool bIsSuccessfulDebuff = false;

    // Debuff configuration
    FScalableFloat DebuffDamage = 0.f;
    FScalableFloat DebuffDuration = 0.f;
    FScalableFloat DebuffFrequency = 0.f;

    // Damage type identification
    TSharedPtr<FGameplayTag> DamageType;

    // Physics impulses
    FVector DeathImpulse = FVector::ZeroVector;
    FVector KnockBackForce = FVector::ZeroVector;
};
```

## How to Use the Enhanced Library

### Basic Multi-Damage Application

```cpp
// In your ability class
void UYourDamageAbility::ApplyDamageToTarget(AActor* Target)
{
    FDamageEffectParams Params = MakeDamageEffectParamsFromClassDefaults(Target);
    UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);
}
```

### Custom Damage Configuration

```cpp
// Create custom damage parameters
FDamageEffectParams CustomParams;
CustomParams.WorldContextObject = GetWorld();
CustomParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
CustomParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
CustomParams.AbilityLevel = GetAbilityLevel();

// Add multiple damage types
FAuraDamageGameplayEffect FireDamage;
FireDamage.DamageEffectClass = FireEffectClass;
FireDamage.Damage.SetValue(50.0f);
FireDamage.DebuffChance.SetValue(30.0f);
CustomParams.DamageType.Add(FAuraGameplayTags::Get().Damage_Fire, FireDamage);

FAuraDamageGameplayEffect LightningDamage;
LightningDamage.DamageEffectClass = LightningEffectClass;
LightningDamage.Damage.SetValue(35.0f);
LightningDamage.KnockBackChance.SetValue(50.0f);
CustomParams.DamageType.Add(FAuraGameplayTags::Get().Damage_Lightning, LightningDamage);

// Apply the damage
UAuraAbilitySystemLibrary::ApplyDamageEffect(CustomParams);
```

### Blueprint Integration

The library methods are exposed to Blueprint:

```cpp
UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InDeathImpulse);

UFUNCTION(BlueprintCallable, Category= "Aura Ability System Library|Gameplay Effects")
static void SetKnockBackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InForce);
```

## Advanced Usage Patterns

### Conditional Damage Application

```cpp
// Apply damage only if certain conditions are met
for(const TTuple<FGameplayTag, FAuraDamageGameplayEffect>& Pair : DamageParams.DamageType)
{
    // Check if target is vulnerable to this damage type
    if (TargetASC->HasMatchingGameplayTag(GetVulnerabilityTag(Pair.Key)))
    {
        // Increase damage for vulnerability
        ModifiedParams.DamageType[Pair.Key].Damage.SetValue(
            Pair.Value.Damage.GetValueAtLevel(Level) * 1.5f
        );
    }
}
```

### Dynamic Effect Class Selection

```cpp
// Choose different effect classes based on game state
FAuraDamageGameplayEffect& FireDamage = DamageParams.DamageType[FireTag];
if (IsRainingWeather())
{
    FireDamage.DamageEffectClass = WeakenedFireEffectClass;
    FireDamage.Damage.SetValue(FireDamage.Damage.GetValueAtLevel(Level) * 0.5f);
}
```

### Chained Effect Application

```cpp
// Apply primary damage, then secondary effects
FGameplayEffectContextHandle Context = UAuraAbilitySystemLibrary::ApplyDamageEffect(PrimaryDamage);

// Use context for secondary effects
if (UAuraAbilitySystemLibrary::IsCriticalHit(Context))
{
    UAuraAbilitySystemLibrary::ApplyDamageEffect(CriticalBonusDamage);
}
```

## Performance Optimizations

### Effect Pooling

```cpp
// Cache frequently used effect specs
TMap<FGameplayTag, FGameplayEffectSpecHandle> CachedEffectSpecs;

FGameplayEffectSpecHandle GetCachedEffectSpec(const FGameplayTag& DamageType, const FAuraDamageGameplayEffect& DamageConfig)
{
    if (!CachedEffectSpecs.Contains(DamageType))
    {
        CachedEffectSpecs.Add(DamageType, CreateEffectSpec(DamageConfig));
    }
    return CachedEffectSpecs[DamageType];
}
```

### Batch Processing

```cpp
// Apply multiple damage types in a single network call
void ApplyBatchDamage(const TArray<FDamageEffectParams>& DamageArray)
{
    for (const FDamageEffectParams& Params : DamageArray)
    {
        ApplyDamageEffect(Params);
    }
}
```

## Best Practices

1. **Centralized Processing**: Always use the library for damage application
2. **Context Preservation**: Maintain effect context for follow-up processing
3. **Network Efficiency**: Batch damage applications when possible
4. **Error Handling**: Check for valid ASCs and effect classes
5. **Performance**: Cache frequently used effect specifications

## Integration with Other Systems

### Damage Calculation Integration

```cpp
// Works seamlessly with ExecCalc_Damage
void UExecCalc_Damage::Execute_Implementation(...)
{
    // Damage calculation uses the same context system
    FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(EffectContextHandle);
    // Process damage based on type...
}
```

### Attribute Set Integration

```cpp
// Attribute set can query damage context
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Data.EffectSpec.GetContext());
        // Handle damage based on type...
    }
}
```

## Troubleshooting

### Common Issues

1. **Effects Not Applying**: Check that effect classes are valid and properly configured
2. **Context Data Lost**: Ensure context is properly passed through the entire pipeline
3. **Network Desync**: Verify all context data is properly serialized
4. **Performance Issues**: Consider batching for multiple simultaneous damage applications

### Debug Tools

```cpp
// Add logging for damage application debugging
UE_LOG(LogAura, Warning, TEXT("Applying %s damage: %.2f"),
    *DamageType.ToString(),
    ScaledDamage);

// Verify context data
if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
{
    UE_LOG(LogAura, Warning, TEXT("Damage Type: %s, Critical: %s"),
        AuraContext->GetDamageType().IsValid() ? *AuraContext->GetDamageType()->ToString() : TEXT("None"),
        AuraContext->IsCriticalHit() ? TEXT("Yes") : TEXT("No"));
}
```

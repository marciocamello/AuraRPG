# Multi-Damage Type System

## Overview

This feature extends the basic damage system to support multiple damage types simultaneously in a single ability, providing much more flexibility than the course's single-damage approach.

## What Was Implemented

### Enhanced Damage Architecture

- **Multiple Damage Types**: Abilities can deal Fire, Lightning, Arcane, and Physical damage simultaneously
- **Individual Effect Classes**: Each damage type can have its own `UGameplayEffect` class
- **Complex Configuration**: Per-damage-type settings for debuffs, knockback, and visual effects
- **Scalable Parameters**: All damage values use `FScalableFloat` for level progression

### Key Structures

#### FAuraDamageGameplayEffect

```cpp
USTRUCT(BlueprintType)
struct FAuraDamageGameplayEffect
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FScalableFloat Damage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FScalableFloat DebuffChance = 20.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FScalableFloat DebuffDamage = 5.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FScalableFloat DebuffFrequency = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FScalableFloat DebuffDuration = 5.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FScalableFloat DeathImpulseMagnitude = 1000.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FVector DeathImpulse = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FScalableFloat KnockBackForceMagnitude = 1000.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FScalableFloat KnockBackChance = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
    FVector KnockBackForce = FVector::ZeroVector;
};
```

#### Enhanced FDamageEffectParams

```cpp
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
    GENERATED_BODY()

    // ... base properties ...

    UPROPERTY(BlueprintReadWrite)
    TMap<FGameplayTag, FAuraDamageGameplayEffect> DamageType;  // Multiple damage types
};
```

## Implementation Details

### Multi-Damage CauseDamage Method

```cpp
void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
    const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

    // Loop through all damage types
    for(TTuple<FGameplayTag, FAuraDamageGameplayEffect> Pair : DamageType)
    {
        const float ScaledDamage = Pair.Value.Damage.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
    }

    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *DamageSpecHandle.Data.Get(),
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
    );
}
```

### Dynamic Impulse Calculation

```cpp
FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
    FDamageEffectParams Params;
    // ... setup base params ...

    if(IsValid(TargetActor))
    {
        FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
        Rotation.Pitch = 45.f;
        const FVector ToTarget = Rotation.Vector();

        // Calculate impulses for each damage type
        for(TTuple<FGameplayTag, FAuraDamageGameplayEffect> Pair : DamageType)
        {
            if (Params.DamageType.Contains(Pair.Key) && DamageType.Contains(Pair.Key))
            {
                Params.DamageType[Pair.Key].DeathImpulse = ToTarget * DamageType[Pair.Key].DeathImpulseMagnitude.Value;
                Params.DamageType[Pair.Key].KnockBackForce = ToTarget * DamageType[Pair.Key].KnockBackForceMagnitude.Value;
            }
        }
    }

    return Params;
}
```

### Damage Query System

```cpp
float UAuraDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageTypeTag) const
{
    for(TTuple<FGameplayTag, FAuraDamageGameplayEffect> Pair : DamageType)
    {
        if(DamageTypeTag == Pair.Key)
        {
            return Pair.Value.Damage.GetValueAtLevel(InLevel);
        }
    }
    return 0.f;
}
```

## How to Create a Multi-Damage Ability

### Step 1: Setup C++ Class

1. **Inherit from AuraDamageGameplayAbility**:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "YourMultiDamageAbility.generated.h"

UCLASS()
class AURA_API UYourMultiDamageAbility : public UAuraDamageGameplayAbility
{
    GENERATED_BODY()

public:
    // Your specific ability logic here

protected:
    // Inherited DamageType TMap will handle multiple damage types
};
```

### Step 2: Blueprint Configuration

1. **Create Gameplay Ability Blueprint**:

   - Base Class: Your C++ class or `AuraDamageGameplayAbility`
   - Open Blueprint editor

2. **Configure Damage Type Map**:

   - Find `DamageType` property in Details panel
   - Add entries for each damage type you want:

   **Example Configuration**:

   ```
   DamageType Map:
   [0] Key: Damage.Fire
       Value:
         - DamageEffectClass: GE_FireDamage_Class
         - Damage: (Curve with values 25, 50, 75...)
         - DebuffChance: (Curve with values 30, 35, 40...)
         - DebuffDamage: (Curve with values 5, 10, 15...)
         - DeathImpulseMagnitude: (Curve with value 1000)

   [1] Key: Damage.Lightning
       Value:
         - DamageEffectClass: GE_LightningDamage_Class
         - Damage: (Curve with values 15, 30, 45...)
         - DebuffChance: (Curve with values 25, 30, 35...)
         - KnockBackChance: (Curve with values 20, 25, 30...)
   ```

3. **Setup Damage Effect Classes**:
   - Create separate GameplayEffect Blueprints for each damage type
   - Base them on your main damage effect class
   - Configure unique properties per damage type

### Step 3: Advanced Configuration

#### Custom Damage Combinations

**Fire + Lightning Spell Example**:

```cpp
// In Blueprint or C++
DamageType.Add(FAuraGameplayTags::Get().Damage_Fire, FireDamageConfig);
DamageType.Add(FAuraGameplayTags::Get().Damage_Lightning, LightningDamageConfig);
```

**Hybrid Physical + Arcane Attack**:

```cpp
DamageType.Add(FAuraGameplayTags::Get().Damage_Physical, PhysicalDamageConfig);
DamageType.Add(FAuraGameplayTags::Get().Damage_Arcane, ArcaneDamageConfig);
```

#### Progressive Scaling

Use different curve progressions for each damage type:

```
Fire Damage: 25 → 50 → 75 → 100 (Linear growth)
Lightning Damage: 10 → 30 → 60 → 100 (Exponential growth)
```

### Step 4: Integration with Ability System Library

The enhanced `ApplyDamageEffect` method automatically handles multiple damage types:

```cpp
FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
    // Automatically loops through all damage types in the map
    for(const TTuple<FGameplayTag, FAuraDamageGameplayEffect>& Pair : DamageEffectParams.DamageType)
    {
        // Creates individual effect specs for each damage type
        // Applies proper scaling, debuffs, and impulses
    }
}
```

## Blueprint Usage Examples

### Example 1: Fire Bolt with Burn

```
DamageType Map:
[0] Key: Damage.Fire
    Value:
      - Damage: 30 base damage
      - DebuffChance: 40% burn chance
      - DebuffDamage: 5 damage per tick
      - DebuffDuration: 3 seconds
      - DebuffFrequency: 1 second intervals
```

### Example 2: Lightning Strike with Stun

```
DamageType Map:
[0] Key: Damage.Lightning
    Value:
      - Damage: 45 base damage
      - DebuffChance: 25% stun chance
      - KnockBackChance: 60%
      - KnockBackForceMagnitude: 1500
```

### Example 3: Hybrid Elemental Blast

```
DamageType Map:
[0] Key: Damage.Fire
    Value: (Fire configuration)
[1] Key: Damage.Lightning
    Value: (Lightning configuration)
[2] Key: Damage.Arcane
    Value: (Arcane configuration)
```

## Best Practices

1. **Balanced Damage Distribution**: When using multiple damage types, balance total damage appropriately
2. **Unique Debuff Types**: Different damage types should have different debuff effects
3. **Visual Consistency**: Use appropriate VFX for each damage type
4. **Performance Consideration**: Limit the number of simultaneous damage types for performance
5. **Clear Progression**: Each damage type should scale meaningfully with level

## Troubleshooting

### Common Issues

1. **No Damage Applied**: Check that GameplayEffect classes are properly set for each damage type
2. **Missing Visual Effects**: Ensure each damage type has appropriate VFX configured
3. **Incorrect Scaling**: Verify FScalableFloat curves are set up correctly
4. **Performance Issues**: Too many damage types may impact performance on weaker hardware

### Debug Tips

- Use `UE_LOG` to output damage calculations
- Check Gameplay Debugger for applied effects
- Verify damage type tags are properly registered
- Test with single damage type first, then add complexity

## Comparison with Course Implementation

| Feature       | Course (Professor)             | Your Implementation                         |
| ------------- | ------------------------------ | ------------------------------------------- |
| Damage Types  | Single damage type per ability | Multiple damage types per ability           |
| Configuration | Individual float properties    | Structured map with complex configs         |
| Scalability   | Basic FScalableFloat           | Full FScalableFloat support per damage type |
| Effects       | One effect class               | Individual effect classes per damage type   |
| Flexibility   | Limited to simple abilities    | Supports complex hybrid abilities           |

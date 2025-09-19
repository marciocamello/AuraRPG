# Creating New C++ Gameplay Abilities

## Overview

This tutorial covers how to create new C++ gameplay abilities in the Aura project, including both active and passive abilities, with proper integration into the Blueprint and ability system.

## Prerequisites

- Basic understanding of Unreal Engine C++
- Familiarity with Gameplay Ability System (GAS)
- Aura project setup complete

## Creating a Basic Damage Ability

### Step 1: Create the Header File

Create `YourNewAbility.h` in `Source/Aura/Public/AbilitySystem/Abilities/`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "YourNewAbility.generated.h"

UCLASS()
class AURA_API UYourNewAbility : public UAuraDamageGameplayAbility
{
    GENERATED_BODY()

public:
    UYourNewAbility();

    // Override description methods for UI
    virtual FString GetDescription(int32 Level, FText Title, FText Description) override;
    virtual FString GetNextLevelDescription(int32 Level, FText Title, FText Description) override;

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Specific")
    FScalableFloat AbilitySpecificValue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Specific")
    float SomeFloatProperty = 100.0f;

    // Core ability execution
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

    // Additional methods as needed
    UFUNCTION(BlueprintCallable, Category = "Your Ability")
    void YourCustomFunction();

private:
    void InternalHelperFunction();
};
```

### Step 2: Implement the CPP File

Create `YourNewAbility.cpp` in `Source/Aura/Private/AbilitySystem/Abilities/`:

```cpp
#include "AbilitySystem/Abilities/YourNewAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

UYourNewAbility::UYourNewAbility()
{
    // Set default values
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Configure ability tags (these should be added to AuraGameplayTags)
    AbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Attack_YourAbility);

    // Set activation requirements
    ActivationOwnedTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputPressed);
    ActivationOwnedTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputHeld);
    ActivationOwnedTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputReleased);
}

FString UYourNewAbility::GetDescription(int32 Level, FText Title, FText Description)
{
    const int32 SpecificValue = AbilitySpecificValue.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float CooldownCost = GetCooldownCost(Level);

    FString Template = "<Title>{Title}</>\n\n"
                       "<Default>{Description}</>\n\n"
                       "<Small>Level:</> <Level>{Level}</>\n"
                       "<Small>Mana Cost:</> <ManaCost>{ManaCost}</>\n"
                       "<Small>Cooldown:</> <Cooldown>{Cooldown}</>\n"
                       "<Small>Special Value:</> <Damage>{SpecificValue}</>";

    Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
    Template = Template.Replace(TEXT("{Description}"), *Description.ToString());
    Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());

    const FString FormattedManaCost = FString::Printf(TEXT("%.1f"), ManaCost);
    Template = Template.Replace(TEXT("{ManaCost}"), *FText::FromString(FormattedManaCost).ToString());
    Template = Template.Replace(TEXT("{Cooldown}"), *FText::FromString(FString::SanitizeFloat(CooldownCost)).ToString());
    Template = Template.Replace(TEXT("{SpecificValue}"), *FText::FromString(FString::FromInt(SpecificValue)).ToString());

    return Template;
}

FString UYourNewAbility::GetNextLevelDescription(int32 Level, FText Title, FText Description)
{
    // Similar implementation to GetDescription but for next level
    const int32 NextLevel = Level + 1;
    const int32 SpecificValue = AbilitySpecificValue.GetValueAtLevel(NextLevel);

    // Same template but with NextLevel values
    // ... implementation similar to GetDescription

    return Template;
}

void UYourNewAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // Your ability logic here
    YourCustomFunction();

    // If this is a damage ability, you can use the inherited damage system
    // Example: Apply damage to a target
    if (AActor* Target = GetTargetFromSomewhere())
    {
        FDamageEffectParams DamageParams = MakeDamageEffectParamsFromClassDefaults(Target);
        UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageParams);
    }

    // End the ability when done
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UYourNewAbility::YourCustomFunction()
{
    // Custom ability logic
    const float SpecificValue = AbilitySpecificValue.GetValueAtLevel(GetAbilityLevel());

    // Do something with the value
    UE_LOG(LogTemp, Warning, TEXT("Your ability activated with value: %f"), SpecificValue);

    // Call helper function
    InternalHelperFunction();
}

void UYourNewAbility::InternalHelperFunction()
{
    // Private helper logic
}
```

### Step 3: Add Gameplay Tags

In `AuraGameplayTags.h`, add your new tags:

```cpp
// In FAuraGameplayTags struct
FGameplayTag Abilities_Attack_YourAbility;
```

In `AuraGameplayTags.cpp`, register the tags:

```cpp
// In InitializeNativeGameplayTags()
GameplayTags.Abilities_Attack_YourAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Attack.YourAbility"),
    FString("Tag for Your New Ability")
);
```

### Step 4: Create Blueprint Asset

1. **Create Gameplay Ability Blueprint**:

   - Right-click in Content Browser
   - Go to Blueprint Class
   - Search for "Your New Ability"
   - Create Blueprint based on your C++ class

2. **Configure Blueprint Properties**:
   - Set `AbilitySpecificValue` curve values
   - Configure `DamageType` map if using damage system
   - Set ability costs, cooldowns, and tags
   - Configure any additional properties

## Creating a Passive Ability

### Step 1: Create Passive Ability Class

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraPassiveAbility.h"
#include "YourPassiveAbility.generated.h"

UCLASS()
class AURA_API UYourPassiveAbility : public UAuraPassiveAbility
{
    GENERATED_BODY()

public:
    virtual FString GetDescription(int32 Level, FText Title, FText Description) override;
    virtual FString GetNextLevelDescription(int32 Level, FText Title, FText Description) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Passive")
    FScalableFloat PassiveValue;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
                            const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo,
                            bool bReplicateEndAbility, bool bWasCancelled) override;
};
```

### Step 2: Implement Passive Ability

```cpp
#include "AbilitySystem/Abilities/YourPassiveAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UYourPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
    {
        // Add your passive tag
        AuraASC->AddLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_YourPassive);

        // Broadcast visual effect activation
        AuraASC->MulticastActivatePassiveEffect(FAuraGameplayTags::Get().Abilities_Passive_YourPassive, true);
    }
}

void UYourPassiveAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
    {
        // Remove your passive tag
        AuraASC->RemoveLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_YourPassive);

        // Broadcast visual effect deactivation
        AuraASC->MulticastActivatePassiveEffect(FAuraGameplayTags::Get().Abilities_Passive_YourPassive, false);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
```

## Advanced Ability Features

### Adding Custom Ability System Component Integration

```cpp
// In your ability class
void UYourNewAbility::ActivateAbility(...)
{
    Super::ActivateAbility(...);

    // Get custom ASC
    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
    {
        // Use custom ASC methods
        AuraASC->AddCharacterAbilityInputPressed(InputTag);

        // Listen for custom delegates
        AuraASC->AbilityStatusChanged.AddUObject(this, &UYourNewAbility::OnAbilityStatusChanged);
    }
}

void UYourNewAbility::OnAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
    // React to ability status changes
}
```

### Blueprint-Callable Functions

```cpp
// Add Blueprint-accessible functions
UFUNCTION(BlueprintCallable, Category = "Your Ability")
float GetCustomValue() const
{
    return AbilitySpecificValue.GetValueAtLevel(GetAbilityLevel());
}

UFUNCTION(BlueprintImplementableEvent, Category = "Your Ability")
void OnAbilitySpecialEvent();

UFUNCTION(BlueprintNativeEvent, Category = "Your Ability")
void HandleCustomLogic();
void HandleCustomLogic_Implementation()
{
    // Default C++ implementation
    UE_LOG(LogTemp, Warning, TEXT("Custom logic executed"));
}
```

### Integration with Animation System

```cpp
// Add animation montage support
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
TObjectPtr<UAnimMontage> AbilityMontage;

void UYourNewAbility::ActivateAbility(...)
{
    Super::ActivateAbility(...);

    // Play animation
    if (AbilityMontage)
    {
        FTaggedMontage TaggedMontage;
        TaggedMontage.Montage = AbilityMontage;
        TaggedMontage.MontageTag = FAuraGameplayTags::Get().CombatSocket_Weapon;

        GetAvatarActorFromActorInfo()->PlayAnimMontage(AbilityMontage);
    }
}
```

## Best Practices

### 1. Proper Inheritance Hierarchy

- **UAuraGameplayAbility**: Base for all abilities
- **UAuraDamageGameplayAbility**: For abilities that deal damage
- **UAuraPassiveAbility**: For passive abilities
- **Custom Base Classes**: Create your own base classes for specific ability types

### 2. Tag Management

- Always use unique tags for each ability
- Register tags in `AuraGameplayTags.cpp`
- Use consistent naming conventions
- Consider tag hierarchies for organization

### 3. Network Considerations

```cpp
// Ensure proper replication
virtual bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const override;

// Use appropriate RPC calls
UFUNCTION(Server, Reliable)
void ServerExecuteAbility();

UFUNCTION(NetMulticast, Reliable)
void MulticastExecuteAbility();
```

### 4. Performance Optimization

```cpp
// Cache frequently accessed data
void UYourNewAbility::ActivateAbility(...)
{
    // Cache avatar actor
    if (!CachedAvatarActor.IsValid())
    {
        CachedAvatarActor = GetAvatarActorFromActorInfo();
    }

    // Use cached reference
    if (CachedAvatarActor.IsValid())
    {
        // Perform operations
    }
}
```

### 5. Error Handling

```cpp
void UYourNewAbility::ActivateAbility(...)
{
    // Validate state before executing
    if (!GetAvatarActorFromActorInfo())
    {
        UE_LOG(LogTemp, Error, TEXT("YourNewAbility: No avatar actor found"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // Check required components
    if (!GetAbilitySystemComponentFromActorInfo())
    {
        UE_LOG(LogTemp, Error, TEXT("YourNewAbility: No ability system component found"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // Continue with ability logic...
}
```

## Common Patterns

### Input-Based Abilities

```cpp
// Handle different input types
virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
```

### Target Acquisition

```cpp
// Add targeting support
UPROPERTY(EditDefaultsOnly, Category = "Targeting")
float TargetingRange = 1000.0f;

AActor* FindBestTarget() const
{
    // Implement targeting logic
    TArray<AActor*> PotentialTargets;
    GetTargetsInRange(PotentialTargets);
    return SelectBestTarget(PotentialTargets);
}
```

### Cost and Cooldown Management

```cpp
// Override cost calculations
virtual float GetManaCost(float InLevel = -1.0f) const override;
virtual float GetCooldownCost(float InLevel = -1.0f) const override;

// Custom resource costs
virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
```

## Troubleshooting

### Common Issues

1. **Ability Not Activating**: Check ability tags and activation requirements
2. **Blueprint Compilation Errors**: Ensure all referenced classes are properly included
3. **Network Desync**: Verify RPC usage and replication settings
4. **Missing Visual Effects**: Check tag matching and component setup

### Debug Tips

- Use `AbilitySystemDebug` console commands
- Add `UE_LOG` statements for debugging
- Check Gameplay Debugger for ability states
- Verify tag registration in editor

This tutorial provides a comprehensive foundation for creating new C++ gameplay abilities in the Aura project. Adapt these patterns to fit your specific ability requirements.

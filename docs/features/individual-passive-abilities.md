# Individual Passive Abilities System

## Overview

This feature extends the base passive ability system by creating individual C++ classes for each passive ability, providing greater flexibility and specialized functionality compared to the course's generic approach.

## What Was Implemented

### Base Class Structure

- **AuraPassiveAbility.h/.cpp**: Base class for all passive abilities
- **Individual Classes**:
  - `UAuraHaloOfProtection`: Damage reduction passive
  - `UAuraLifeSiphon`: Health restoration on damage dealt
  - `UAuraManaSiphon`: Mana restoration on damage dealt

### Key Features

1. **Individual Descriptions**: Each ability has custom description methods
2. **Scalable Values**: Uses `FScalableFloat` for level-based progression
3. **Gameplay Tag Management**: Proper tag addition/removal on activation/deactivation
4. **Visual Effects Integration**: Individual Niagara components for each ability

## Implementation Details

### C++ Class Structure

```cpp
// Base Class: AuraPassiveAbility.h
class AURA_API UAuraPassiveAbility : public UAuraGameplayAbility
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(...) override;
    void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};

// Individual Class Example: AuraHaloOfProtection.h
class AURA_API UAuraHaloOfProtection : public UAuraPassiveAbility
{
    GENERATED_BODY()

public:
    virtual FString GetDescription(int32 Level, FText Title, FText Description) override;
    virtual FString GetNextLevelDescription(int32 Level, FText Title, FText Description) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FScalableFloat PassiveAbilityCurve;

    virtual void ActivateAbility(...) override;
    virtual void EndAbility(...) override;
};
```

### Visual Effects Integration

Each passive ability has its own Niagara component in the character:

```cpp
// In AuraCharacterBase.h
UPROPERTY(VisibleAnywhere)
TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

UPROPERTY(VisibleAnywhere)
TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

UPROPERTY(VisibleAnywhere)
TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;
```

## How to Create a New Passive Ability

### Step 1: Create C++ Class

1. **Create Header File** (`YourPassiveAbility.h`):

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
```

2. **Create Implementation File** (`YourPassiveAbility.cpp`):

```cpp
#include "AbilitySystem/Abilities/YourPassiveAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

FString UYourPassiveAbility::GetDescription(int32 Level, FText Title, FText Description)
{
    const int32 YourValue = PassiveAbilityCurve.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float CoolDownCost = GetCooldownCost(Level);

    FString Template = "<Title>{Title}</>\n\n"
                       "<Default>{Description}</>\n\n"
                       "<Small>Level:</> <Level>{Level}</>\n"
                       "<Small>Mana Cost:</> <ManaCost>{ManaCost}</>\n"
                       "<Small>Cooldown:</> <Cooldown>{Cooldown}</>";

    Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
    Template = Template.Replace(TEXT("{YourValue}"), *FText::FromString(FString::FromInt(YourValue)).ToString());
    Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());

    const FString FormattedManaCost = FString::Printf(TEXT("%.1f"), ManaCost);
    Template = Template.Replace(TEXT("{ManaCost}"), *FText::FromString(FormattedManaCost).ToString());
    Template = Template.Replace(TEXT("{Cooldown}"), *FText::FromString(FString::SanitizeFloat(CoolDownCost)).ToString());

    return Template.Replace(TEXT("{Description}"), *Description.ToString());
}

void UYourPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
    {
        // Add your specific gameplay tag
        AuraASC->AddLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_YourAbility);
    }
}

void UYourPassiveAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
    {
        // Remove your specific gameplay tag
        AuraASC->RemoveLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_YourAbility);
    }
}
```

### Step 2: Add Gameplay Tags

In `AuraGameplayTags.cpp`, add your new tag:

```cpp
// In InitializeNativeGameplayTags()
GameplayTags.Abilities_Passive_YourAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.YourAbility"),
    FString("Tag granted when having Your Ability passive ability")
);
```

### Step 3: Create Niagara Component (Optional)

In `AuraCharacterBase.h`:

```cpp
UPROPERTY(VisibleAnywhere)
TObjectPtr<UPassiveNiagaraComponent> YourAbilityNiagaraComponent;
```

In `AuraCharacterBase.cpp` constructor:

```cpp
YourAbilityNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("YourAbilityNiagaraComponent"));
YourAbilityNiagaraComponent->SetupAttachment(EffectAttachComponent);
```

### Step 4: Blueprint Configuration

1. **Create Gameplay Ability Blueprint**:

   - Create new Blueprint based on your C++ class
   - Set `PassiveAbilityCurve` values
   - Configure ability tags and costs

2. **Set Niagara Effect** (if using visual effects):

   - In Character Blueprint, find your Niagara component
   - Set `PassiveSpellTag` to match your gameplay tag
   - Assign appropriate Niagara system

3. **Add to Character Class**:
   - In your Character Class Data Asset
   - Add the new ability to `StartupPassiveAbilities` array

## Best Practices

1. **Unique Gameplay Tags**: Always create unique tags for each passive ability
2. **Scalable Values**: Use `FScalableFloat` for level-based progression
3. **Description Templates**: Follow the established template format for consistency
4. **Visual Feedback**: Consider adding Niagara effects for visual feedback
5. **Proper Cleanup**: Always remove tags in `EndAbility`

## Troubleshooting

### Common Issues

1. **Wrong Gameplay Tags**: Make sure each passive uses its own unique tag
2. **Missing Tag Registration**: Ensure tags are properly registered in `AuraGameplayTags.cpp`
3. **Visual Effects Not Showing**: Check that `PassiveSpellTag` matches the ability's gameplay tag
4. **Ability Not Activating**: Verify the ability is added to `StartupPassiveAbilities`

### Debug Tips

- Use `UE_LOG` to debug tag addition/removal
- Check ability activation in the Gameplay Debugger
- Verify Niagara component setup in Blueprint details

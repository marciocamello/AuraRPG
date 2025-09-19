# Enhanced Passive Niagara Component System

## Overview

This improvement enhances the visual feedback system for passive abilities by providing individual, properly managed Niagara components for each passive ability with automatic activation/deactivation based on ability state.

## What Was Improved

### Individual Component Architecture

- **Dedicated Components**: Each passive ability gets its own `UPassiveNiagaraComponent`
- **Automatic Management**: Components activate/deactivate based on ability tags
- **Proper Attachment**: All components properly attached to a centralized `EffectAttachComponent`
- **Network Compatibility**: Proper replication and network synchronization

### Enhanced PassiveNiagaraComponent

```cpp
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
    GENERATED_BODY()

public:
    UPassiveNiagaraComponent();

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag PassiveSpellTag;  // Tag to listen for

protected:
    virtual void BeginPlay() override;
    void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
};
```

## Implementation Details

### Component Registration System

```cpp
void UPassiveNiagaraComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
    {
        // Direct registration for immediate ASC availability
        AuraASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
    }
    else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
    {
        // Deferred registration for cases where ASC isn't ready yet
        CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
        {
            if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC))
            {
                AuraASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
            }
        });
    }
}
```

### Tag-Based Activation

```cpp
void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
    if (AbilityTag.MatchesTag(PassiveSpellTag))
    {
        if (bActivate && !IsActive())
        {
            Activate();
        }
        else
        {
            Deactivate();
        }
    }
}
```

### Character Integration

```cpp
// AuraCharacterBase.h
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
private:
    // Centralized attachment point for all effects
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> EffectAttachComponent;

    // Individual passive ability Niagara components
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;
};

// AuraCharacterBase.cpp Constructor
AAuraCharacterBase::AAuraCharacterBase()
{
    // Create attachment point
    EffectAttachComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EffectAttachPoint"));
    EffectAttachComponent->SetupAttachment(GetRootComponent());

    // Create individual passive Niagara components
    HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("HaloOfProtectionNiagaraComponent"));
    HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);

    LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("LifeSiphonNiagaraComponent"));
    LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);

    ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("ManaSiphonNiagaraComponent"));
    ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
}
```

## How to Add New Passive Visual Effects

### Step 1: Add Component to Character

1. **Declare in Header** (`AuraCharacterBase.h`):

```cpp
UPROPERTY(VisibleAnywhere)
TObjectPtr<UPassiveNiagaraComponent> YourNewPassiveNiagaraComponent;
```

2. **Create in Constructor** (`AuraCharacterBase.cpp`):

```cpp
YourNewPassiveNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>(TEXT("YourNewPassiveNiagaraComponent"));
YourNewPassiveNiagaraComponent->SetupAttachment(EffectAttachComponent);
```

### Step 2: Configure Component in Blueprint

1. **Open Character Blueprint**
2. **Find Your Component** in the Components panel
3. **Set PassiveSpellTag**:

   - Select your component
   - In Details panel, find `PassiveSpellTag`
   - Set to your ability's gameplay tag (e.g., `Abilities.Passive.YourAbility`)

4. **Configure Niagara System**:
   - Set `Niagara System` to your VFX asset
   - Configure `Auto Activate` to `false` (important!)
   - Adjust transform/scale as needed

### Step 3: Ensure Ability Broadcasts Events

Your passive ability must broadcast activation events:

```cpp
void UYourPassiveAbility::ActivateAbility(...)
{
    Super::ActivateAbility(...);

    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(...))
    {
        // This triggers the Niagara component activation
        AuraASC->MulticastActivatePassiveEffect(YourAbilityTag, true);
    }
}

void UYourPassiveAbility::EndAbility(...)
{
    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(...))
    {
        // This triggers the Niagara component deactivation
        AuraASC->MulticastActivatePassiveEffect(YourAbilityTag, false);
    }

    Super::EndAbility(...);
}
```

## Blueprint Configuration Guide

### Setting Up Individual Components

1. **Character Blueprint Setup**:

   ```
   Character_BP
   ├── Capsule Component
   ├── Mesh
   ├── EffectAttachComponent
   │   ├── HaloOfProtectionNiagaraComponent
   │   │   └── PassiveSpellTag: "Abilities.Passive.HaloOfProtection"
   │   │   └── Niagara System: NS_HaloOfProtection
   │   ├── LifeSiphonNiagaraComponent
   │   │   └── PassiveSpellTag: "Abilities.Passive.LifeSiphon"
   │   │   └── Niagara System: NS_LifeSiphon
   │   └── ManaSiphonNiagaraComponent
   │       └── PassiveSpellTag: "Abilities.Passive.ManaSiphon"
   │       └── Niagara System: NS_ManaSiphon
   ```

2. **Component Configuration**:
   - **Auto Activate**: Set to `false` for all passive components
   - **PassiveSpellTag**: Must exactly match the ability's gameplay tag
   - **Transform**: Position relative to EffectAttachComponent
   - **Niagara System**: Assign appropriate VFX asset

### Creating VFX Assets

1. **Create Niagara System**:

   - Right-click in Content Browser
   - Create → FX → Niagara System
   - Choose appropriate template (e.g., Simple Sprite Burst)

2. **Configure for Passive Use**:

   - Set looping if effect should persist
   - Configure spawn rate for continuous effects
   - Set appropriate colors/materials for ability theme

3. **Performance Optimization**:
   - Use LOD system for distance-based quality
   - Limit particle count for passive effects
   - Consider using simple materials

## Advanced Features

### Dynamic Effect Scaling

You can modify effects based on ability level:

```cpp
void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
    if (AbilityTag.MatchesTag(PassiveSpellTag))
    {
        if (bActivate && !IsActive())
        {
            // Get ability level and scale effect
            if (UAuraAbilitySystemComponent* ASC = GetOwnerASC())
            {
                FGameplayAbilitySpec* Spec = ASC->GetSpecFromAbilityTag(AbilityTag);
                if (Spec)
                {
                    float Level = Spec->Level;
                    SetFloatParameter(FName("AbilityLevel"), Level);
                    SetFloatParameter(FName("EffectScale"), Level * 0.2f + 0.8f);
                }
            }
            Activate();
        }
        else
        {
            Deactivate();
        }
    }
}
```

### Multi-Tag Support

For abilities that might have multiple related effects:

```cpp
// In PassiveNiagaraComponent.h
UPROPERTY(EditDefaultsOnly)
FGameplayTagContainer PassiveSpellTags;  // Multiple tags

// In OnPassiveActivate
void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
    if (PassiveSpellTags.HasTag(AbilityTag))
    {
        // Handle activation for any matching tag
    }
}
```

## Best Practices

1. **Performance Optimization**:

   - Use `bAutoActivate = false` for all passive components
   - Keep particle counts reasonable for persistent effects
   - Use LOD systems for complex effects

2. **Tag Management**:

   - Ensure PassiveSpellTag exactly matches ability tags
   - Use consistent naming conventions
   - Test tag matching in gameplay debugger

3. **Visual Consistency**:

   - Use similar color schemes for related abilities
   - Scale effects appropriately for character size
   - Consider visual clarity in combat situations

4. **Network Considerations**:
   - Effects automatically replicate through component system
   - Avoid frequent activation/deactivation
   - Use `MulticastActivatePassiveEffect` for network sync

## Troubleshooting

### Common Issues

1. **Effect Not Activating**:

   - Check PassiveSpellTag matches ability tag exactly
   - Verify ability is calling `MulticastActivatePassiveEffect`
   - Ensure component AutoActivate is set to false

2. **Effect Stays Active**:

   - Check ability's `EndAbility` calls deactivation
   - Verify tag removal in ability system
   - Check for multiple abilities with same tag

3. **Network Sync Issues**:
   - Ensure using `MulticastActivatePassiveEffect`
   - Check component replication settings
   - Verify ability system component replication

### Debug Tips

- Use Gameplay Debugger to check active tags
- Add UE_LOG statements in activation functions
- Check component attachment in Blueprint debugger
- Verify Niagara system assignments in editor

## Performance Considerations

1. **Particle Limits**: Keep passive effects lightweight (< 50 particles)
2. **Update Frequency**: Use lower update rates for background effects
3. **Culling**: Implement distance-based culling for passive effects
4. **Memory**: Pool Niagara components if creating many characters

# Blueprint Integration Guide

## Overview

This guide covers how to properly integrate C++ abilities and systems with Blueprints in the Aura project, including setup, configuration, and best practices for the enhanced systems.

## Setting Up C++ Classes in Blueprint

### Creating Ability Blueprints from C++ Classes

#### Step 1: Create Blueprint Class

1. **Right-click in Content Browser**
2. **Select "Blueprint Class"**
3. **Search for your C++ class name** (e.g., "AuraHaloOfProtection")
4. **Create the Blueprint**

#### Step 2: Configure Base Properties

In the Blueprint editor, you'll find these inherited properties:

```cpp
// From AuraGameplayAbility
- Ability Tags
- Activation Owned Tags
- Activation Required Tags
- Activation Blocked Tags
- Cancel Abilities With Tag
- Block Abilities With Tag

// From AuraDamageGameplayAbility (if applicable)
- DamageEffectClass
- DamageType (TMap<FGameplayTag, FAuraDamageGameplayEffect>)

// From AuraPassiveAbility (if applicable)
- PassiveAbilityCurve
```

## Configuring Multi-Damage Abilities

### Setting Up DamageType Map

The `DamageType` property is a map that supports multiple damage types:

#### Basic Single Damage Configuration

```
DamageType Map:
├── [0] Key: Damage.Fire
│   └── Value: FAuraDamageGameplayEffect
│       ├── DamageEffectClass: GE_FireDamage
│       ├── Damage: (Curve Table Entry)
│       │   └── Level 1: 25.0
│       │   └── Level 2: 50.0
│       │   └── Level 3: 75.0
│       ├── DebuffChance: (Curve Table Entry)
│       │   └── Level 1: 20.0
│       │   └── Level 2: 25.0
│       │   └── Level 3: 30.0
│       ├── DebuffDamage: (Curve Table Entry)
│       └── DebuffDuration: (Curve Table Entry)
```

#### Advanced Multi-Damage Configuration

```
DamageType Map:
├── [0] Key: Damage.Fire
│   └── Value: FAuraDamageGameplayEffect
│       ├── DamageEffectClass: GE_FireDamage
│       ├── Damage: (25, 50, 75, 100...)
│       ├── DebuffChance: (30, 35, 40, 45...)
│       ├── DebuffDamage: (5, 8, 12, 16...)
│       └── DebuffDuration: (3.0, 3.5, 4.0, 4.5...)
├── [1] Key: Damage.Lightning
│   └── Value: FAuraDamageGameplayEffect
│       ├── DamageEffectClass: GE_LightningDamage
│       ├── Damage: (15, 30, 45, 60...)
│       ├── KnockBackChance: (20, 25, 30, 35...)
│       ├── KnockBackForceMagnitude: (800, 1000, 1200, 1400...)
│       └── DeathImpulseMagnitude: (1000, 1200, 1400, 1600...)
└── [2] Key: Damage.Arcane
    └── Value: FAuraDamageGameplayEffect
        ├── DamageEffectClass: GE_ArcaneDamage
        ├── Damage: (20, 40, 60, 80...)
        ├── DebuffChance: (15, 20, 25, 30...)
        └── DebuffFrequency: (1.0, 0.8, 0.6, 0.4...)
```

### Configuring FScalableFloat Values

#### Using Direct Values

```
Damage (FScalableFloat):
├── Value: 50.0 (Fixed value for all levels)
└── Curve: None
```

#### Using Curve Tables

```
Damage (FScalableFloat):
├── Value: 1.0 (Multiplier)
└── Curve:
    ├── Curve Table: CT_AbilityDamage
    ├── Row Name: FireBolt_Damage
    └── Preview: Shows curve values per level
```

#### Creating Curve Tables

1. **Create Curve Table Asset**:

   - Right-click → Miscellaneous → Curve Table
   - Choose Float as row structure

2. **Configure Curve Data**:
   ```
   Row Name: FireBolt_Damage
   Level 1: 25.0
   Level 2: 50.0
   Level 3: 75.0
   Level 4: 100.0
   Level 5: 125.0
   ```

## Passive Ability Blueprint Setup

### Individual Passive Ability Configuration

#### Step 1: Create Passive Ability Blueprint

1. **Base Class**: Your C++ passive ability class
2. **Configure PassiveAbilityCurve**:
   ```
   PassiveAbilityCurve:
   ├── Value: 1.0
   └── Curve:
       ├── Curve Table: CT_PassiveAbilities
       ├── Row Name: HaloOfProtection_Reduction
       └── Values: 5, 10, 15, 20, 25... (damage reduction %)
   ```

#### Step 2: Set Ability Properties

```
Ability Configuration:
├── Ability Tags:
│   └── Abilities.Passive.HaloOfProtection
├── Activation Owned Tags:
│   └── (Usually empty for passives)
├── Activation Required Tags:
│   └── (Usually empty for passives)
└── Instancing Policy: Instanced Per Actor
```

#### Step 3: Configure Costs (Optional)

```
Costs:
├── Cost Gameplay Effect Class: (Usually None for passives)
├── Cooldown Gameplay Effect Class: (Usually None for passives)
└── Additional Costs: (Usually None for passives)
```

### Character Blueprint Integration

#### Adding Passive Components

In your Character Blueprint:

```
Character Blueprint Components:
├── EffectAttachComponent (Scene Component)
│   ├── HaloOfProtectionNiagaraComponent (PassiveNiagaraComponent)
│   │   ├── PassiveSpellTag: Abilities.Passive.HaloOfProtection
│   │   ├── Niagara System: NS_HaloOfProtection
│   │   └── Auto Activate: False
│   ├── LifeSiphonNiagaraComponent (PassiveNiagaraComponent)
│   │   ├── PassiveSpellTag: Abilities.Passive.LifeSiphon
│   │   ├── Niagara System: NS_LifeSiphon
│   │   └── Auto Activate: False
│   └── ManaSiphonNiagaraComponent (PassiveNiagaraComponent)
│       ├── PassiveSpellTag: Abilities.Passive.ManaSiphon
│       ├── Niagara System: NS_ManaSiphon
│       └── Auto Activate: False
```

#### Setting Up Niagara Components

For each PassiveNiagaraComponent:

1. **Component Settings**:

   ```
   PassiveNiagaraComponent:
   ├── PassiveSpellTag: (Must match ability tag exactly)
   ├── Niagara System: (Your VFX asset)
   ├── Auto Activate: False (Important!)
   ├── Activation:
   │   └── Activate on BeginPlay: False
   └── Transform: (Position relative to EffectAttachComponent)
   ```

2. **Visual Configuration**:
   ```
   Niagara System Settings:
   ├── Looping: True (for persistent effects)
   ├── Auto Destroy: False
   ├── Spawn Rate: Moderate (20-100 particles/sec)
   └── Material: Appropriate themed material
   ```

## Character Class Data Asset Setup

### Adding Abilities to Character Classes

#### Startup Abilities Configuration

```
Character Class Info:
├── Startup Abilities:
│   ├── [0] GA_MeleeAttack
│   ├── [1] GA_FireBolt
│   └── [2] GA_Electrocute
└── Startup Passive Abilities:
    ├── [0] GA_HaloOfProtection
    ├── [1] GA_LifeSiphon
    └── [2] GA_ManaSiphon
```

#### Primary Attributes Setup

```
Primary Attributes:
├── Strength: 10
├── Intelligence: 15
├── Resilience: 12
├── Vigor: 8
└── (Class-specific variations)
```

#### Secondary Attribute Coefficients

```
Secondary Attribute Coefficients:
├── Maximum Health Coefficient: 2.5
├── Maximum Mana Coefficient: 3.0
├── Armor Coefficient: 1.5
├── Armor Penetration Coefficient: 1.0
├── Block Chance Coefficient: 0.5
├── Critical Hit Chance Coefficient: 0.8
├── Critical Hit Damage Coefficient: 1.2
├── Critical Hit Resistance Coefficient: 0.6
├── Health Regeneration Coefficient: 1.0
├── Mana Regeneration Coefficient: 1.5
└── Max Movement Speed Coefficient: 1.0
```

## Ability Info Data Asset Configuration

### Setting Up Ability Information

```
Ability Info Array:
├── [0] Fire Bolt Info:
│   ├── Ability Tag: Abilities.Fire.FireBolt
│   ├── Input Tag: InputTag.1
│   ├── Status Tag: Abilities.Status.Unlocked
│   ├── Ability Type Tag: Abilities.Type.Offensive
│   ├── Cooldown Tag: Cooldown.Fire.FireBolt
│   ├── Damage Type Tag: Damage.Fire
│   ├── Description: "Launches a bolt of fire..."
│   ├── Next Level Description: "Next level increases..."
│   ├── Ability Icon: T_FireBolt_Icon
│   └── Background Material: M_AbilityBackground_Fire
├── [1] Halo of Protection Info:
│   ├── Ability Tag: Abilities.Passive.HaloOfProtection
│   ├── Input Tag: InputTag.Passive.1
│   ├── Status Tag: Abilities.Status.Unlocked
│   ├── Ability Type Tag: Abilities.Type.Passive
│   ├── Description: "Reduces incoming damage..."
│   └── Ability Icon: T_HaloOfProtection_Icon
```

## Widget Blueprint Integration

### Ability Description Widgets

#### Creating Description Widget

1. **Widget Blueprint Setup**:

   ```
   Ability Description Widget:
   ├── Rich Text Block (Description)
   ├── Image (Ability Icon)
   ├── Text Block (Ability Name)
   └── Progress Bar (Cooldown/Mana)
   ```

2. **Binding to C++ Functions**:
   ```cpp
   // In your widget's Blueprint
   Event Pre Construct:
   ├── Get Ability Info
   ├── Call GetDescription(Level, Title, Description)
   ├── Set Rich Text
   └── Set Icon Texture
   ```

#### Rich Text Formatting

The description system supports rich text formatting:

```cpp
// Formatting tags available in descriptions
<Title>Ability Name</>           // Large, bold text
<Default>Description text</>     // Normal description text
<Small>Label:</>                 // Small label text
<Level>5</>                     // Level styling
<ManaCost>25.5</>              // Mana cost styling
<Cooldown>3.0</>               // Cooldown styling
<Damage>50</>                  // Damage value styling
```

### Spell Menu Integration

#### Connecting Abilities to UI

```cpp
// In your spell menu widget
UFUNCTION(BlueprintCallable)
void InitializeSpellMenu()
{
    // Get all ability specs from ASC
    if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
    {
        AuraASC->ForEachAbility([this](const FGameplayAbilitySpec& AbilitySpec)
        {
            // Create UI elements for each ability
            CreateAbilityWidget(AbilitySpec);
        });
    }
}
```

#### Ability Slot Management

```cpp
// Assign abilities to input slots
void AssignAbilityToSlot(FGameplayTag AbilityTag, FGameplayTag SlotTag)
{
    if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
    {
        if (FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(AbilityTag))
        {
            AuraASC->AssignSlotToAbility(*AbilitySpec, SlotTag);
        }
    }
}
```

## Best Practices for Blueprint Integration

### 1. Tag Consistency

- **Always verify tag spelling** in Blueprint configurations
- **Use the same tags** across C++, Blueprints, and Data Assets
- **Test tag matching** using Gameplay Debugger

### 2. Performance Considerations

```cpp
// Cache frequently accessed data in Blueprints
Event BeginPlay:
├── Get Ability System Component (Cache)
├── Get Character Class Info (Cache)
└── Initialize UI Elements (One-time setup)
```

### 3. Network Compatibility

- **Ensure all configurable properties replicate** properly
- **Test multiplayer scenarios** with different configurations
- **Use appropriate RPC calls** for UI updates

### 4. Error Handling

```cpp
// Add validation in Blueprint Event Graph
Is Valid (Ability System Component):
├── True: Continue with ability logic
└── False: Log error and skip functionality
```

### 5. Modular Design

- **Create reusable widget components** for common UI elements
- **Use inheritance hierarchies** for similar ability types
- **Implement interfaces** for common functionality

## Common Blueprint Patterns

### Conditional Ability Activation

```cpp
// Blueprint Event Graph example
On Input Action Triggered:
├── Get Ability by Tag
├── Check Can Activate Ability
├── True: Try Activate Ability
└── False: Show Error Message
```

### Dynamic UI Updates

```cpp
// Update UI when ability changes
On Ability Status Changed:
├── Get Ability Info
├── Update Description Text
├── Update Icon
├── Update Cooldown Display
└── Refresh Widget Appearance
```

### Level-Based Configuration

```cpp
// Scale effects based on ability level
Event Construct:
├── Get Ability Level
├── Get Curve Value at Level
├── Set Material Parameters
└── Update Visual Scale
```

## Troubleshooting Common Issues

### Blueprint Compilation Errors

1. **Missing C++ Class References**: Ensure C++ code compiles first
2. **Invalid Property References**: Check that exposed UPROPERTY macros are correct
3. **Tag Mismatches**: Verify gameplay tags are registered and spelled correctly

### Runtime Issues

1. **Abilities Not Activating**: Check tag configurations and ability system setup
2. **UI Not Updating**: Verify delegate bindings and event subscriptions
3. **Visual Effects Not Appearing**: Check Niagara component configurations and tags
4. **Network Desync**: Ensure proper replication settings on configurable properties

### Performance Problems

1. **Blueprint Tick Events**: Avoid heavy operations in Tick
2. **Excessive Widget Updates**: Cache values and update only when necessary
3. **Complex Curve Evaluations**: Consider pre-calculating values for common levels

This guide provides comprehensive coverage of Blueprint integration for the enhanced Aura systems. Use these patterns and practices to create robust, maintainable ability configurations.

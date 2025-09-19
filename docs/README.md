# Documentation Index

## Overview

This documentation covers the enhanced features and improvements implemented in the Aura RPG project beyond the course material from Stephen Ulibarri's Gameplay Ability System course.

## Features

### [Individual Passive Abilities System](features/individual-passive-abilities.md)

Enhanced passive ability architecture with individual C++ classes for each passive ability, providing specialized functionality and better maintainability.

**Key Improvements:**

- Individual C++ classes for each passive ability (HaloOfProtection, LifeSiphon, ManaSiphon)
- Custom description generation system
- Scalable values with FScalableFloat
- Dedicated visual effects integration
- Proper gameplay tag management

### [Multi-Damage Type System](features/multi-damage-type-system.md)

Advanced damage system supporting multiple damage types simultaneously in a single ability, far beyond the course's single-damage approach.

**Key Improvements:**

- Multiple damage types per ability (Fire + Lightning + Arcane + Physical)
- Individual effect classes per damage type
- Complex per-damage-type configuration
- Dynamic impulse and knockback calculation
- Flexible damage query system

## Improvements

### [Enhanced Passive Niagara Component System](improvements/enhanced-passive-niagara-system.md)

Improved visual feedback system with individual, automatically managed Niagara components for each passive ability.

**Key Improvements:**

- Individual components for each passive ability
- Automatic activation/deactivation based on ability tags
- Proper network synchronization
- Centralized attachment system
- Performance optimizations

### [Advanced Damage Effect Library](improvements/advanced-damage-effect-library.md)

Enhanced AuraAbilitySystemLibrary with sophisticated damage effect application supporting the multi-damage type architecture.

**Key Improvements:**

- Centralized damage processing
- Multi-effect application support
- Enhanced context system
- Complex damage parameter handling
- Network-optimized implementation

## Tutorials

### [Creating New C++ Gameplay Abilities](tutorials/creating-cpp-abilities.md)

Comprehensive guide for creating new C++ gameplay abilities in the Aura project, covering both active and passive abilities.

**Topics Covered:**

- Basic damage ability creation
- Passive ability implementation
- Advanced features integration
- Best practices and patterns
- Network considerations

### [Blueprint Integration Guide](tutorials/blueprint-integration-guide.md)

Complete guide for integrating C++ abilities and systems with Blueprints, including configuration and best practices.

**Topics Covered:**

- Creating ability Blueprints from C++ classes
- Multi-damage ability configuration
- Passive ability Blueprint setup
- Widget Blueprint integration
- Performance optimization

## Quick Reference

### Gameplay Tags Structure

```
Abilities
├── Fire
│   └── FireBolt
├── Lightning
│   └── Electrocute
├── Passive
│   ├── HaloOfProtection
│   ├── LifeSiphon
│   └── ManaSiphon
└── Type
    ├── Offensive
    ├── Passive
    └── None

Damage
├── Fire
├── Lightning
├── Arcane
└── Physical
```

### Key Classes Overview

| Class                        | Purpose                        | Base Class             |
| ---------------------------- | ------------------------------ | ---------------------- |
| `UAuraPassiveAbility`        | Base for all passive abilities | `UAuraGameplayAbility` |
| `UAuraHaloOfProtection`      | Damage reduction passive       | `UAuraPassiveAbility`  |
| `UAuraLifeSiphon`            | Health restoration passive     | `UAuraPassiveAbility`  |
| `UAuraManaSiphon`            | Mana restoration passive       | `UAuraPassiveAbility`  |
| `UAuraDamageGameplayAbility` | Base for damage abilities      | `UAuraGameplayAbility` |
| `UPassiveNiagaraComponent`   | Visual effects for passives    | `UNiagaraComponent`    |

### Configuration Examples

#### Multi-Damage Ability Configuration

```cpp
DamageType Map:
[0] Key: Damage.Fire
    Value: FAuraDamageGameplayEffect
    - Damage: 50.0
    - DebuffChance: 30.0
    - DebuffDamage: 10.0

[1] Key: Damage.Lightning
    Value: FAuraDamageGameplayEffect
    - Damage: 35.0
    - KnockBackChance: 25.0
    - KnockBackForceMagnitude: 1200.0
```

#### Passive Niagara Component Setup

```cpp
Component Configuration:
- PassiveSpellTag: "Abilities.Passive.HaloOfProtection"
- Niagara System: NS_HaloOfProtection
- Auto Activate: False
- Parent: EffectAttachComponent
```

## Course vs Enhanced Implementation

| Feature           | Course Implementation          | Enhanced Implementation               |
| ----------------- | ------------------------------ | ------------------------------------- |
| Passive Abilities | Generic base class only        | Individual specialized classes        |
| Damage System     | Single damage type per ability | Multiple damage types per ability     |
| Visual Effects    | Basic Niagara integration      | Individual managed components         |
| Damage Library    | Simple damage application      | Advanced multi-damage processing      |
| Configuration     | Basic Blueprint setup          | Complex multi-parameter configuration |
| Scalability       | Limited flexibility            | Highly extensible architecture        |

## Getting Started

1. **For New Passive Abilities**: Start with [Individual Passive Abilities System](features/individual-passive-abilities.md)
2. **For Multi-Damage Abilities**: Follow [Multi-Damage Type System](features/multi-damage-type-system.md)
3. **For C++ Development**: Use [Creating New C++ Gameplay Abilities](tutorials/creating-cpp-abilities.md)
4. **For Blueprint Configuration**: Reference [Blueprint Integration Guide](tutorials/blueprint-integration-guide.md)

## Support and Troubleshooting

Each document includes:

- ✅ **Implementation Details**: Complete code examples
- ✅ **Step-by-Step Guides**: How to create new features
- ✅ **Best Practices**: Recommended approaches
- ✅ **Troubleshooting**: Common issues and solutions
- ✅ **Performance Tips**: Optimization recommendations

## Version Information

- **Unreal Engine**: 5.6
- **Project**: AuraRPG
- **Course Base**: Stephen Ulibarri's GAS Course (up to Section 27)
- **Enhancement Level**: Advanced (Beyond Course Material)

---

**Note**: This documentation covers enhancements and extensions beyond the original course material. For basic GAS concepts and initial implementation, refer to the course content.

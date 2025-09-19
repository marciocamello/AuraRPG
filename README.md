# Aura RPG - Top Down RPG with Gameplay Ability System (GAS)

## 📖 About the Project

**Aura RPG** is a top-down RPG game developed in **Unreal Engine 5.6** following the course [Unreal Engine 5 - Gameplay Ability System - Top Down RPG](https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg) by instructor Stephen Ulibarri. The project demonstrates the complete implementation of an RPG system using Unreal Engine's **Gameplay Ability System (GAS)**, focusing on AAA code architecture, C++ programming, and SOLID principles.

## 🎯 Project Goals

- Implement a complete RPG using the Gameplay Ability System
- Demonstrate AAA-quality code architecture
- Properly balance the use of C++ and Blueprints
- Create scalable, modular, and expandable systems
- Implement combat mechanics, experience, leveling, and multiplayer

## 🏗️ Technical Architecture

### Implemented Core Systems

#### **Gameplay Ability System (GAS)**

- **Ability System Component**: `UAuraAbilitySystemComponent`
- **Attribute Set**: `UAuraAttributeSet` with primary, secondary, and resistance attributes
- **Gameplay Effects**: Complete system of effects for attribute modification
- **Gameplay Tags**: Comprehensive tag system for identifying abilities, attributes, and states

#### **Attribute System**

**Primary Attributes:**

- `Strength` - Increases physical damage
- `Intelligence` - Increases magical damage
- `Resilience` - Increases Armor and Armor Penetration
- `Vigor` - Increases Max Health

**Secondary Attributes:**

- `Armor` - Reduces damage taken, improves Block Chance
- `Armor Penetration` - Ignores percentage of enemy Armor
- `Block Chance` - Chance to reduce incoming damage by half
- `Critical Hit Chance` - Chance for critical damage
- `Critical Hit Damage` - Bonus damage on critical hits
- `Health/Mana Regeneration` - Regeneration per second
- `Max Health/Mana` - Maximum values

**Resistance Attributes:**

- `Fire Resistance` - Resistance to fire damage
- `Lightning Resistance` - Resistance to lightning damage
- `Arcane Resistance` - Resistance to arcane damage
- `Physical Resistance` - Resistance to physical damage

#### **Character Class System**

- **Elementalist** - Elemental magic specialist
- **Warrior** - Melee combat fighter
- **Ranger** - Archer and ranged combat

### UI/UX System

#### **Implemented Widget Controllers**

- `UOverlayWidgetController` - Main HUD
- `UAttributeMenuWidgetController` - Attribute menu
- `USpellMenuWidgetController` - Spell menu
- `UAuraWidgetController` - Base class for all controllers

#### **HUD System**

- Main HUD with Health/Mana display
- Floating text system for damage
- Visual indicators for critical hits and blocks
- Experience and level bars

## 🎮 Implemented Gameplay Systems

### ✅ **Completed Sections (Through Section 27)**

#### **Section 1-5: Foundations**

- ✅ Top-Down project setup from scratch
- ✅ Enemy and item selection system with outline effects
- ✅ Character class hierarchy with OOP
- ✅ Introduction to GAS and core classes

#### **Section 6-10: Core GAS Implementation**

- ✅ Custom GAS components
- ✅ Attribute Sets and attribute replication
- ✅ Important settings for all GAS classes
- ✅ Complete primary and secondary attribute system

#### **Section 11-15: Gameplay Effects and Tags**

- ✅ Gameplay Effects for attribute modification
- ✅ Comprehensive Gameplay Tags system
- ✅ Effect application pipeline
- ✅ Damage and combat calculations

#### **Section 16-20: UI and Menus**

- ✅ Complex and organized UI system
- ✅ MVC and MVVM paradigms
- ✅ Complete attribute and spell menus
- ✅ Ability upgrade system

#### **Section 21-25: Combat and Abilities**

- ✅ Complete RPG combat system
- ✅ Different damage types and resistances
- ✅ Debuffs corresponding to damage types
- ✅ Complete visual feedback (floating text, knockback, stun)

#### **Section 26-27: Advanced Systems**

- ✅ Experience and level-up system
- ✅ Attribute and spell point system
- ✅ Passive abilities implemented
- ✅ Enemy AI with Behavior Trees and EQS

### **Implemented Abilities**

#### **🔥 Fire Spells**

- ✅ **FireBolt** - Fire projectile with multiple projectiles per level
  - Scalable damage per level
  - Homing projectile system
  - Chance to apply burn debuff

#### **⚡ Lightning Spells**

- ✅ **Electrocute** - Beam-type spell with chain lightning
  - Beam targeting system
  - Chain to multiple enemies
  - Stun debuff application

#### **🛡️ Passive Abilities**

- ✅ **Halo of Protection** - Passive protection
- ✅ **Life Siphon** - Life steal on attacks
- ✅ **Mana Siphon** - Mana steal on attacks

### 🚧 **Current Section: Section 28 - Arcane Shards**

The next implementation will be **Arcane Shards**, an arcane damage spell that will demonstrate:

- Multiple arcane projectile system
- Advanced targeting mechanics
- Integration with arcane resistance system
- Unique visual effects for arcane magic

## 🚀 **Enhanced Features & Improvements**

This project includes several enhancements and improvements beyond the course material, documented in detail:

### **📚 [Complete Documentation](docs/README.md)**

Comprehensive documentation covering all enhanced features, improvements, and tutorials.

### **🎯 Enhanced Features**

#### **[Individual Passive Abilities System](docs/features/individual-passive-abilities.md)**

- Individual C++ classes for each passive ability
- Specialized functionality per ability type
- Custom description generation system
- Enhanced visual effects integration

#### **[Multi-Damage Type System](docs/features/multi-damage-type-system.md)**

- Support for multiple damage types in a single ability
- Complex damage configuration with individual effect classes
- Dynamic impulse and knockback calculations
- Advanced damage query and processing system

### **⚡ System Improvements**

#### **[Enhanced Passive Niagara Component System](docs/improvements/enhanced-passive-niagara-system.md)**

- Individual Niagara components for each passive ability
- Automatic activation/deactivation based on gameplay tags
- Improved network synchronization
- Performance optimizations for visual effects

#### **[Advanced Damage Effect Library](docs/improvements/advanced-damage-effect-library.md)**

- Sophisticated damage effect application system
- Centralized damage processing with multi-effect support
- Enhanced context system for complex damage scenarios
- Network-optimized implementation

### **🛠️ Development Guides**

#### **[Creating New C++ Gameplay Abilities](docs/tutorials/creating-cpp-abilities.md)**

- Complete guide for creating new C++ abilities
- Best practices and design patterns
- Integration with the ability system
- Network considerations and optimization

#### **[Blueprint Integration Guide](docs/tutorials/blueprint-integration-guide.md)**

- Comprehensive Blueprint configuration guide
- Multi-damage ability setup
- Widget and UI integration
- Performance optimization techniques

### **📊 Course vs Enhanced Implementation**

| System                | Course Implementation          | Enhanced Implementation            |
| --------------------- | ------------------------------ | ---------------------------------- |
| **Passive Abilities** | Generic base class             | Individual specialized classes     |
| **Damage System**     | Single damage type per ability | Multiple simultaneous damage types |
| **Visual Effects**    | Basic Niagara integration      | Individual managed components      |
| **Damage Processing** | Simple application             | Advanced multi-effect system       |
| **Configuration**     | Basic Blueprint setup          | Complex multi-parameter system     |

### 🔮 **Remaining Sections (Section 28+)**

#### **Section 28-30: Advanced Spells**

- 🚧 **Arcane Shards** - Multiple arcane projectiles with advanced targeting
- 📋 **Fire Blast** - Area-of-effect fire spell with explosion mechanics
- 📋 **Advanced Spell Mechanics** - Complex spell interactions and combinations

#### **Section 31-35: Save System & Level Transitions**

- 📋 **Save/Load System** - Complete game progress saving
- 📋 **Level Transitions** - Seamless map transitions and loading
- 📋 **Game State Management** - Persistent world state handling
- 📋 **Asset Streaming** - Optimized asset loading and management
- 📋 **Performance Optimization** - Memory and CPU optimization techniques

#### **Section 36-40: Advanced Engine Features**

- 📋 **Custom Async Tasks** - Advanced asynchronous operations
- 📋 **Ability Tasks** - Custom gameplay ability tasks
- 📋 **Blueprint Function Libraries** - Reusable Blueprint functions
- 📋 **Asset Managers** - Advanced asset management systems
- 📋 **Custom Gameplay Effect Contexts** - Extended GAS functionality

#### **Section 41-45: Networking & Multiplayer**

- 📋 **Net Serialization** - Advanced networking optimization
- 📋 **Struct Ops Type Traits** - Custom data type optimization
- 📋 **Multiplayer Combat** - Networked combat systems
- 📋 **Server Authority** - Authoritative server implementations
- 📋 **Client Prediction** - Smooth multiplayer experience

#### **Section 46-50: Polish & Advanced Topics**

- 📋 **C++ Lambdas** - Modern C++ techniques in UE5
- 📋 **Game Mechanics Algorithms** - Advanced mathematical implementations
- 📋 **Debugging Tools** - Custom debugging and profiling tools
- 📋 **Code Architecture Patterns** - Advanced design patterns
- 📋 **Final Polish & Optimization** - Production-ready optimization

#### **Bonus Sections: Production Pipeline**

- 📋 **Build Automation** - Automated build and deployment
- 📋 **Quality Assurance** - Testing frameworks and methodologies
- 📋 **Performance Profiling** - Advanced performance analysis
- 📋 **Platform Optimization** - Console and PC optimization
- 📋 **Post-Launch Support** - Maintenance and update strategies

## 🎨 Assets and Resources

The project includes a complete asset pack created specifically for the course:

### **Characters**

- **Aura** - Main character
- **Enemies**: Goblin Warriors, Goblin Rangers, Goblin Shaman
- **Demons**: Red Demons, Black Demons
- **Others**: Ghoul, Shroom

### **Environments**

- Modular dungeon system
- Waypoint shrines and obelisks
- High-quality HUD textures

### **Visual Effects (Niagara)**

- Explosions and impacts
- Level up effects
- Projectiles (rocks, electricity beams, fireballs)
- Stun and debuff effects
- Flame and star particles

### **Audio**

- Sound effects for enemies
- Footsteps and movement sounds
- Spell casting sounds
- Combat feedback audio

### **UI/UX Assets**

- High-quality buttons
- Frames and borders
- Dynamically animated spell globes
- Progress bars and XP bar
- Textures for different UI states

## 🔧 Technologies and Tools

- **Unreal Engine**: 5.6
- **Language**: C++ 17
- **IDE**: Visual Studio / Rider (compatible)
- **Build System**: Unreal Build Tool
- **Version Control**: Git
- **Architecture**: SOLID principles

## 📁 Project Structure

```
Aura/
├── Config/                     # Project configurations
├── Content/                    # Game assets
│   ├── Assets/                # Models, textures, materials
│   ├── Blueprints/            # Game blueprints
│   │   ├── Actor/            # Actors (Projectiles, etc.)
│   │   ├── Character/        # Character blueprints
│   │   ├── UI/               # User interface
│   │   └── AbilitySystem/    # GAS blueprints
│   └── Maps/                 # Game maps
├── Data/                      # CSV/JSON data tables
├── Source/Aura/              # C++ source code
│   ├── Private/
│   │   ├── AbilitySystem/    # Ability system
│   │   │   ├── Abilities/    # Spell implementations
│   │   │   ├── Data/         # Data classes
│   │   │   └── ExecCalc/     # Execution calculations
│   │   ├── Character/        # Character classes
│   │   ├── Player/           # Player system
│   │   ├── UI/               # Interface system
│   │   │   ├── HUD/          # HUD principal
│   │   │   ├── Widget/       # Widgets customizados
│   │   │   └── WidgetController/ # Controllers MVC
│   │   ├── Actor/            # Actors do jogo
│   │   ├── Game/             # Game mode e regras
│   │   │   ├── HUD/          # Main HUD
│   │   │   ├── Widget/       # Custom widgets
│   │   │   └── WidgetController/ # MVC controllers
│   │   ├── Actor/            # Game actors
│   │   ├── Game/             # Game mode and rules
│   │   └── Interaction/      # Interaction interfaces
│   └── Public/               # Corresponding headers
└── Plugins/                  # Used plugins
```

## 🚀 Planned Future Features

### **Gameplay Expansions**

- [ ] **Fire Blast** - Area-of-effect fire spell
- [ ] Inventory and item system
- [ ] Crafting system
- [ ] Quest system
- [ ] Guild/group system
- [ ] PvP arena system

### **Technical Improvements**

- [ ] Advanced save/load system
- [ ] Level transitions
- [ ] Achievement system
- [ ] Analytics and telemetry
- [ ] Modding system
- [ ] Performance profiling tools

### **Additional Content**

- [ ] Additional maps
- [ ] New enemy types
- [ ] Elaborate boss fights
- [ ] Cutscenes and narrative
- [ ] Companion pet system
- [ ] Mount system

## 📈 Publishing Plans

### **Phase 1: Complete Prototype**

- Finish all course sections
- Implement all planned spells
- Complete save/load system
- General polish and bug fixes

### **Phase 2: Alpha Release**

- Gameplay balancing
- Extensive testing
- Community feedback
- Performance optimizations

### **Phase 3: Beta Release**

- Additional content
- Complete multiplayer system
- Final UI/UX
- Marketing and community building

### **Phase 4: Launch**

- **Steam**: Main release
- **Epic Games Store**: Additional distribution
- **Itch.io**: Indie version
- **Console Ports**: PlayStation, Xbox, Nintendo Switch

## 🛠️ Development Setup

### **Prerequisites**

```bash
# Unreal Engine 5.6
# Visual Studio 2022 or Rider
# Git for version control
# 16GB+ RAM recommended
# Dedicated GPU (GTX 1060 or better)
```

### **How to Build**

```bash
# Clone the repository
git clone https://github.com/marciocamello/AuraRPG.git

# Open Aura.uproject
# Unreal Engine will generate project files
# Build through editor or IDE
```

### **Available Build Tasks**

The project includes configured tasks for different builds:

- **Development Build**: For general development
- **Debug Build**: For detailed debugging
- **DebugGame Build**: Optimized debug for gameplay
- **Test Build**: For automated testing
- **Shipping Build**: Final build for distribution

## 🤝 Contributing

This is an educational project based on Stephen Ulibarri's course. Contributions are welcome for:

- Performance improvements
- Bug fixes
- Documentation
- New experimental features
- Testing and quality assurance

### **Contribution Guidelines**

1. Fork the project
2. Create a feature branch (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -m 'Add new feature'`)
4. Push to the branch (`git push origin feature/new-feature`)
5. Open a Pull Request

## 📚 Learning Resources

### **Original Course**

- [Unreal Engine 5 - GAS - Top Down RPG](https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg)
- Instructor: Stephen Ulibarri (Druid Mechanics)
- 100+ hours of content
- No filler, complete professional project

### **Useful Documentation**

- [Unreal Engine GAS Documentation](https://docs.unrealengine.com/5.3/en-US/gameplay-ability-system-for-unreal-engine/)
- [GAS Community Wiki](https://github.com/tranek/GASDocumentation)
- [Unreal Engine C++ Reference](https://docs.unrealengine.com/5.3/en-US/API/)

### **Communities**

- [Druid Mechanics Discord](https://discord.gg/druidmechanics)
- [Unreal Engine Discord](https://discord.gg/unrealengine)
- [r/unrealengine](https://reddit.com/r/unrealengine)

## 📄 License

This project is developed for educational purposes following Stephen Ulibarri's course. The included assets are provided as part of the course. The source code is available under educational license for students and developers.

## 👥 Credits

- **Original Course**: Stephen Ulibarri (Druid Mechanics)
- **Development**: Marcio Camello
- **Assets**: Provided by the original course
- **Engine**: Epic Games (Unreal Engine 5.6)

## 📞 Contact

- **GitHub**: [@marciocamello](https://github.com/marciocamello)
- **Project**: [AuraRPG Repository](https://github.com/marciocamello/AuraRPG)

---

⚡ **"Empowering developers to create AAA-quality games with clean, scalable architecture"** ⚡

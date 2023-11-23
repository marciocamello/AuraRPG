// Copyright Axchemy Games


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	FString Template = "<Title>{Title}</>\n\n<Default>{Description}</>\n\n<Default>Damage:</> <Damage>{Damage}</>\n<Level>Level:</> <Damage>{Level}</>";

	// TODO: get this title string from ability info
	Template = Template.Replace(TEXT("{Title}"), *FText::FromString("FIRE BOLT").ToString());
	
	Template = Template.Replace(TEXT("{Damage}"), *FText::FromString(FString::FromInt(Damage)).ToString());
	Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
	
	if(Level == 1)
	{
		// TODO: get this description string from ability info
		return Template.Replace(TEXT("{Description}"), *FText::FromString("Launch a bolt of fire, exploding on impact.").ToString());
	}
	else
	{
		const int32 NumProjectilesByLevel = FMath::Min(Level, NumProjectiles);
		
		// TODO: get this description string from ability info
		const FText FormattedText = FText::Format(FText::FromString("Launches {0} bolts of fire, exploding on impact."), FText::AsNumber(NumProjectilesByLevel));
		
		return Template.Replace(TEXT("{Description}"), *FormattedText.ToString());
	}
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	FString Template = "<Title>{Title}</>\n\n<Default>{Description}</>\n\n<Default>Damage:</> <Damage>{Damage}</>\n<Level>Level:</> <Damage>{Level}</>";

	// TODO: get this title string from ability info
	Template = Template.Replace(TEXT("{Title}"), *FText::FromString("NEXT LEVEL").ToString());
	
	Template = Template.Replace(TEXT("{Damage}"), *FText::FromString(FString::FromInt(Damage)).ToString());
	Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
	
	const int32 NumProjectilesByLevel = FMath::Min(Level, NumProjectiles);
	
	// TODO: get this description string from ability info
	const FText FormattedText = FText::Format(FText::FromString("Launches {0} bolts of fire, exploding on impact."), FText::AsNumber(NumProjectilesByLevel));
	
	return Template.Replace(TEXT("{Description}"), *FormattedText.ToString());
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FTaggedMontage& AttackMontage, bool bOverridePitch , float PitchOverride)
{
	if(ProjectileClass == nullptr) return;
	
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocationFromAttackMontage(
		GetAvatarActorFromActorInfo(),
		AttackMontage
	);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if(bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	  
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.ImpactPoint = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
		
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	for(TTuple<FGameplayTag, FScalableFloat>& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}
		
	Projectile->DamageEffectSpecHandle = SpecHandle;
		
	Projectile->FinishSpawning(SpawnTransform);
}

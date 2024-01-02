// Copyright Axchemy Games


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"


FString UAuraFireBolt::GetDescription(int32 Level, FText Title, FText Description)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDownCost = GetCooldownCost(Level);
	
	FString Template = "<Title>{Title}</>\n\n"
						"<Default>{Description}</>\n\n"
						"<Small>Level:</> <Level>{Level}</>\n"
						"<Small>Mana Cost:</> <ManaCost>{ManaCost}</>\n"
						"<Small>Cooldown:</> <Cooldown>{Cooldown}</>\n\n"
						"<Default>Damage:</> <Damage>{Damage}</>";

	Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
	
	Template = Template.Replace(TEXT("{Damage}"), *FText::FromString(FString::FromInt(Damage)).ToString());
	Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
	
	const FString FormattedManaCost = FString::Printf(TEXT("%.1f"), ManaCost);
	Template = Template.Replace(TEXT("{ManaCost}"), *FText::FromString(FormattedManaCost).ToString());
	Template = Template.Replace(TEXT("{Cooldown}"), *FText::FromString(FString::SanitizeFloat(CoolDownCost)).ToString());
	
	if(Level == 1)
	{
		// TODO: get this description string from ability info
		return Template.Replace(TEXT("{Description}"), *Description.ToString());
	}
	else
	{
		const int32 NumProjectilesByLevel = FMath::Min(Level, NumProjectiles);
		const FText FormattedText = FText::Format(Description, FText::AsNumber(NumProjectilesByLevel));
		return Template.Replace(TEXT("{Description}"), *FormattedText.ToString());
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level, FText Title, FText Description)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDownCost = GetCooldownCost(Level);
	
	FString Template = "<Title>{Title}</>\n\n"
						"<Default>{Description}</>\n\n"
						"<Small>Level:</> <Level>{Level}</>\n"
						"<Small>Mana Cost:</> <ManaCost>{ManaCost}</>\n"
						"<Small>Cooldown:</> <Cooldown>{Cooldown}</>\n\n"
						"<Default>Damage:</> <Damage>{Damage}</>";

	Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
	
	Template = Template.Replace(TEXT("{Damage}"), *FText::FromString(FString::FromInt(Damage)).ToString());
	Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
	
	const FString FormattedManaCost = FString::Printf(TEXT("%.1f"), ManaCost);
	Template = Template.Replace(TEXT("{ManaCost}"), *FText::FromString(FormattedManaCost).ToString());
	Template = Template.Replace(TEXT("{Cooldown}"), *FText::FromString(FString::SanitizeFloat(CoolDownCost)).ToString());
	
	const int32 NumProjectilesByLevel = FMath::Min(Level, NumProjectiles);
	const FText FormattedText = FText::Format(Description, FText::AsNumber(NumProjectilesByLevel));
	return Template.Replace(TEXT("{Description}"), *FormattedText.ToString());
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FTaggedMontage& AttackMontage,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocationFromAttackMontage(
		GetAvatarActorFromActorInfo(),
		AttackMontage
	);
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if(bOverridePitch) Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for(FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
	  
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}

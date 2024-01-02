// Copyright Axchemy Games


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


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
	
	TArray<FVector> Directions = UAuraAbilitySystemLibrary::EvenlyRotatedVectors(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for(FVector& Direction : Directions)
	{
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			SocketLocation,
			SocketLocation + Direction * 75.f,
			5,
			FLinearColor::Red,
			120,
			2.f
		);
	}

	for(FRotator& Rot : Rotations)
	{
		const FVector Start = SocketLocation + FVector(0.f,0.f,5.f);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			Start + Rot.Vector() * 75.f,
			5,
			FLinearColor::Blue,
			120,
			2.f
		);
	}
	
	/*const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2, FVector::UpVector);
	
	//NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	if(NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for(int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start = SocketLocation + FVector(0.f,0.f,5.f);
			const FVector End = Start + Direction * 75.f;
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(),
				Start,
				End,
				5,
				FLinearColor::Red,
				120,
				2.f
			);
		}
	}
	else
	{
		//Single projectile
		const FVector Start = SocketLocation + FVector(0.f,0.f,5.f);
		const FVector End = Start + Forward * 75.f;
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			End,
			5,
			FLinearColor::Red,
			120,
			2.f
		);
	}
	
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation,
		SocketLocation + Forward * 100.f,
		5,
		FLinearColor::White,
		120,
		2.f
	);
	
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation,
		SocketLocation + LeftOfSpread * 100.f,
		5,
		FLinearColor::Gray,
		120,
		1.f
		);
	
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation,
		SocketLocation + RightOfSpread * 100.f,
		5,
		FLinearColor::Gray,
		120,
		1.f
	);*/
}

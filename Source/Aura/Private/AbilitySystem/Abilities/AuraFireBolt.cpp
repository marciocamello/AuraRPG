// Copyright Axchemy Games


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"


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
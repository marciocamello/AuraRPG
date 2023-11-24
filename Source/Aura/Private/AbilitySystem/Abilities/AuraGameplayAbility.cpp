// Copyright Axchemy Games


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level, FText Title, FText Description)
{
	if(!Description.IsEmpty() && !Title.IsEmpty())
	{
		FString Template = "<Title>{Title}</>\n\n"
						"<Default>{Description}</>\n\n"
						"<Small>Level:</> <Level>{Level}</>";
		
		Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
		Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
		return Template.Replace(TEXT("{Description}"), *Description.ToString());
	}
	return FString();
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level, FText Title, FText Description)
{
	if(!Description.IsEmpty() && !Title.IsEmpty())
	{
		FString Template = "<Title>{Title}</>\n\n"
						"<Default>{Description}</>\n\n"
						"<Small>Level:</> <Level>{Level}</>";
		
		Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
		Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
		return Template.Replace(TEXT("{Description}"), *Description.ToString());
	}
	return FString();
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level, FText Title, FText Description)
{
	if(Description.IsEmpty() && Title.IsEmpty())
	{
		return FString::Printf(TEXT("<Default>Spell Locked Until </><Small>Level:</> <Level>%d</>"), Level);
	}
	else
	{
		FString Template = "<Title>{Title}</>\n\n"
							"<Default>{Description}</>";
		
		Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
		return Template.Replace(TEXT("{Description}"), *Description.ToString());
	}
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for(const FGameplayModifierInfo& Modifier : CostEffect->Modifiers)
		{
			if(Modifier.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldownCost(float InLevel) const
{
	float CoolDownCost = 0.f;
	if(const UGameplayEffect* CooldownEffect = GetCostGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CoolDownCost);
	}
	return CoolDownCost;
}

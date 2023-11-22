// Copyright Axchemy Games


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

/*
 * Usage:
 * TArray<FText> TextValues;
 * TextValues.Add(FText::FromString("Firebolt"));
 * TextValues.Add(FText::FromString("This spell is very powerful"));
 * TextValues.Add(FText::FromString("2"));
 * TextValues.Add(FText::FromString("10"));
 * TextValues.Add(FText::FromString("10"));
 * FText Template = FText::FromString("<Title>{Title}</> \n<Default>{Description}</> \n<Default>Level: </><Level>{Level}</> \n<Default>Damage: </><Damage>{Damage}</> \n<Default>Cooldown: </><Cooldown>{Cooldown}</>");
 * GetFormattedDescription(Template, TextValues);
 */
FString UAuraGameplayAbility::GetFormattedDescription(const FString Template, TArray<FText> TextValues)
{
	FFormatNamedArguments Args;

	for (int32 i = 0; i < TextValues.Num(); ++i)
	{
		Args.Add(FString::Printf(TEXT("{%d}"), i), FText::FromString(TextValues[i].ToString()));
	}

	return FText::Format(FText::FromString(Template), Args).ToString();
}

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage </>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

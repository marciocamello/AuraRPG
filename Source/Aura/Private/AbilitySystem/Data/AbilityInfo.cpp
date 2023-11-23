// Copyright Axchemy Games


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilityInformation)
	{
		if (AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo();
}

FAuraAbilityInfoDescription UAbilityInfo::FindAbilityInfoDescriptionForType(const FGameplayTag& AbilityTag, const EAuraAbilityInfoDescriptionType& DescriptionType,
	bool bLogNotFound) const
{
	const FAuraAbilityInfo AbilityInfo = FindAbilityInfoForTag(AbilityTag, bLogNotFound);
	for (const FAuraAbilityInfoDescription& Description : AbilityInfo.Descriptions)
	{
		if (Description.DescriptionType == DescriptionType)
		{
			return Description;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for DescriptionType [%s] on AbilityInfo [%s]"), *UEnum::GetValueAsString(DescriptionType), *GetNameSafe(this));
	}

	return FAuraAbilityInfoDescription();
}
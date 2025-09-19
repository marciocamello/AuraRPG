// Copyright Axchemy Games

#include "AbilitySystem/Abilities/AuraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

FString UAuraPassiveAbility::GetBaseTemplate(int32 Level, FText Title, FText Description)
{
	FString BaseTemplate = "<Title>{Title}</>\n\n"
						   "<Default>{Description}</>\n\n"
						   "<Small>Level:</> <Level>{Level}</>\n";

	BaseTemplate = BaseTemplate.Replace(TEXT("{Title}"), *Title.ToString());
	BaseTemplate = BaseTemplate.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
	BaseTemplate = BaseTemplate.Replace(TEXT("{Description}"), *Description.ToString());

	return BaseTemplate;
}

FString UAuraPassiveAbility::GetDescription(int32 Level, FText Title, FText Description)
{
	FString Template = GetBaseTemplate(Level, Title, Description);
	Template += GetSpecificEffectLine(Level);
	return Template;
}

FString UAuraPassiveAbility::GetNextLevelDescription(int32 Level, FText Title, FText Description)
{
	return GetDescription(Level, Title, Description);
}

void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
										  const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent *AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->DeactivatePassiveAbility.AddUObject(this, &UAuraPassiveAbility::ReceiveDeactivate);
	}
}

void UAuraPassiveAbility::ReceiveDeactivate(const FGameplayTag &AbilityTag)
{
	if (GetAssetTags().HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

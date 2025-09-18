// Copyright Axchemy Games


#include "AbilitySystem/Abilities/AuraManaSiphon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

FString UAuraManaSiphon::GetDescription(int32 Level, FText Title, FText Description)
{
    const int32 DamageReduce = PassiveAbilityCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDownCost = GetCooldownCost(Level);
	
	FString Template = "<Title>{Title}</>\n\n"
						"<Default>{Description}</>\n\n"
						"<Small>Level:</> <Level>{Level}</>\n"
						"<Small>Mana Cost:</> <ManaCost>{ManaCost}</>\n"
						"<Small>Cooldown:</> <Cooldown>{Cooldown}</>";

	Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
	Template = Template.Replace(TEXT("{DamageReduction}"), *FText::FromString(FString::FromInt(DamageReduce)).ToString());
	Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
	
	const FString FormattedManaCost = FString::Printf(TEXT("%.1f"), ManaCost);
	Template = Template.Replace(TEXT("{ManaCost}"), *FText::FromString(FormattedManaCost).ToString());
	Template = Template.Replace(TEXT("{Cooldown}"), *FText::FromString(FString::SanitizeFloat(CoolDownCost)).ToString());
	
	return Template.Replace(TEXT("{Description}"), *Description.ToString());
}

FString UAuraManaSiphon::GetNextLevelDescription(int32 Level, FText Title, FText Description)
{
    const int32 DamageReduce = PassiveAbilityCurve.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDownCost = GetCooldownCost(Level);
	
	FString Template = "<Title>{Title}</>\n\n"
						"<Default>{Description}</>\n\n"
						"<Small>Level:</> <Level>{Level}</>\n"
						"<Small>Mana Cost:</> <ManaCost>{ManaCost}</>\n"
						"<Small>Cooldown:</> <Cooldown>{Cooldown}</>";

	Template = Template.Replace(TEXT("{Title}"), *Title.ToString());
	Template = Template.Replace(TEXT("{DamageReduction}"), *FText::FromString(FString::FromInt(DamageReduce)).ToString());
	Template = Template.Replace(TEXT("{Level}"), *FText::FromString(FString::FromInt(Level)).ToString());
	
	const FString FormattedManaCost = FString::Printf(TEXT("%.1f"), ManaCost);
	Template = Template.Replace(TEXT("{ManaCost}"), *FText::FromString(FormattedManaCost).ToString());
	Template = Template.Replace(TEXT("{Cooldown}"), *FText::FromString(FString::SanitizeFloat(CoolDownCost)).ToString());
	
	return Template.Replace(TEXT("{Description}"), *Description.ToString());
}

void UAuraManaSiphon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->AddLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_HaloOfProtection);
	}
}

void UAuraManaSiphon::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->RemoveLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_HaloOfProtection);
	}
}
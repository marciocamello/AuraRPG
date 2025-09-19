// Copyright Axchemy Games

#include "AbilitySystem/Abilities/AuraHaloOfProtection.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraHaloOfProtection::UAuraHaloOfProtection()
{
	PassiveType = EPassiveAbilityType::Protection;
}

FString UAuraHaloOfProtection::GetSpecificEffectLine(int32 Level)
{
	const int32 DamageReduction = PassiveAbilityCurve.GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Small>Damage Reduction:</> <Highlight>%d%%</>"), DamageReduction);
}

void UAuraHaloOfProtection::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
											const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
											const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent *AuraASC = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->AddLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_HaloOfProtection);
	}
}

void UAuraHaloOfProtection::EndAbility(const FGameplayAbilitySpecHandle Handle,
									   const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
									   bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UAuraAbilitySystemComponent *AuraASC = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->RemoveLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_HaloOfProtection);
	}
}

// Copyright Axchemy Games

#include "AbilitySystem/Abilities/AuraManaSiphon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraManaSiphon::UAuraManaSiphon()
{
	PassiveType = EPassiveAbilityType::ManaSteal;
}

FString UAuraManaSiphon::GetSpecificEffectLine(int32 Level)
{
	const int32 ManaSteal = PassiveAbilityCurve.GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Small>Mana Steal:</> <Highlight>%d%%</>"), ManaSteal);
}

void UAuraManaSiphon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
									  const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
									  const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent *AuraASC = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->AddLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_ManaSiphon);
	}
}

void UAuraManaSiphon::EndAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
								 bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UAuraAbilitySystemComponent *AuraASC = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->RemoveLooseGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_ManaSiphon);
	}
}